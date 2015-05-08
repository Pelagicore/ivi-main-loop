#include "ivi-main-loop.h"
#include "ivi-main-loop-log.h"

#include <sys/socket.h>
#include <fcntl.h>
#include "unistd.h"

void fd_set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

using namespace ivi;

/**
 * Basic example using the main loop abstraction
 */
int main(int argc, const char * *argv)
{
    DefaultDispatcherType mainLoop;

    int pipes[2];
    if (pipe(pipes) != 0) {
        log_error() << "Can not create pipe";
        exit(-1);
    }

    int &pipeIn = pipes[0];
    int &pipeOut = pipes[1];
    fd_set_non_blocking(pipeIn);
    fd_set_non_blocking(pipeOut);

    TimeOutEventSource *timeOutSource = mainLoop.newTimeoutEventSource([&]() {
                log_debug() << "Writing to pipe ";

                char bytes[64] = {};
                if (write(pipeOut, bytes, sizeof(bytes)) != sizeof(bytes)) {
                    log_error() << "Can not write data";
                    exit(-1);
                }

                return TimeOutEventSource::ReportStatus::KEEP_ENABLED;
            }, 1000);
    timeOutSource->enable();

    TimeOutEventSource *timeOutSourceClose = mainLoop.newTimeoutEventSource([&]() {
                log_debug() << "Closing pipe";
                close(pipeOut);
                return TimeOutEventSource::ReportStatus::DISABLE;
            }, 5000);
    timeOutSourceClose->enable();

    IdleEventSource *idleSource = mainLoop.newIdleEventSource([&]() {
                static int i = 0;
                i++;
                if (i % 500000 == 0) {
                    log_debug() << "idle called " << i << " times";
                }

                return IdleEventSource::ReportStatus::KEEP_ENABLED;
            });
    idleSource->enable();

    TimeOutEventSource *timeOutSource3 = mainLoop.newTimeoutEventSource([&]() {
                log_debug() << "Stopping idle source ";
                idleSource->disable();
                return TimeOutEventSource::ReportStatus::DISABLE;
            }, 2000);
    timeOutSource3->enable();

    FileDescriptorWatchEventSource *fdInputSource = mainLoop.newFileDescriptorWatchEventSource([&](
                FileDescriptorWatchEventSource::Event e) {
                log_debug() << "Data received ";

                char bytes[64];

                auto r = read(pipeIn, bytes, sizeof(bytes));
                if (r < 0) {
                    log_error() << "Can not read data";
                    exit(-1);
                } else {
                    log_debug() << r << " bytes read";
                }

                return FileDescriptorWatchEventSource::ReportStatus::KEEP_ENABLED;

            }, pipeIn, FileDescriptorWatchEventSource::Event::READ_AVAILABLE);
    fdInputSource->enable();

    FileDescriptorWatchEventSource *fdHangUpSource = mainLoop.newFileDescriptorWatchEventSource([&](
                FileDescriptorWatchEventSource::Event e) {
                log_debug() << "Hang up detected => exit main loop";
                mainLoop.quit();
                return FileDescriptorWatchEventSource::ReportStatus::DISABLE;
            }, pipeIn, FileDescriptorWatchEventSource::Event::HANG_UP);
    fdHangUpSource->enable();

    // Run the main loop. The method will return after the quit() method has been called
    mainLoop.run();

    delete timeOutSource;
    delete fdInputSource;
    delete fdHangUpSource;

    log_debug() << "Terminated";

    return 0;
}
