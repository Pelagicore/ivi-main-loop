/**
 * Basic example using the main loop abstraction.
 * That example creates a pipe where bytes are regularly written and read, using the various kind of event sources available.
 * An idle event source is also used to be notified whenever nothing has to be done.
 */

#include <sys/socket.h>
#include <fcntl.h>
#include <stdlib.h>
#include "unistd.h"

#include "ivi-main-loop.h"
#include "ivi-main-loop-log.h"

#include "../c-library/c-library.h"


void fd_set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

using namespace ivi_main_loop;

int main(int argc, const char * *argv)
{
    DefaultEventDispatcherTypes::EventDispatcher mainLoop;
    auto &sourceManager = mainLoop.getSourceManager();

    int pipes[2];
    if (pipe(pipes) != 0) {
        log_error() << "Can not create pipe";
        exit(-1);
    }

    int &pipeIn = pipes[0];
    int &pipeOut = pipes[1];
    fd_set_non_blocking(pipeIn);
    fd_set_non_blocking(pipeOut);

    /// Create a timeout which triggers the writing to the pipe
    DefaultEventDispatcherTypes::TimeOutEventSource timeOutSource(sourceManager, [&]() {

                char bytes[64] = {};
                auto n = write(pipeOut, bytes, sizeof(bytes));
                if (n == sizeof(bytes)) {
                    log_debug() << "Written " << n << " bytes to pipe";
                } else {
                    log_error() << "Can not write data to pipe";
                    mainLoop.quit();
                }

                return TimeOutEventSource::ReportStatus::KEEP_ENABLED;
            }, 1000);
    timeOutSource.enable();

    /// Create a timeout which triggers the closing of the pipe
    DefaultEventDispatcherTypes::TimeOutEventSource timeOutSourceClose(sourceManager, [&]() {
                log_debug() << "Closing pipe";
                close(pipeOut);
                return TimeOutEventSource::ReportStatus::DISABLE;
            }, 5000);
    timeOutSourceClose.enable();

    /// Create a timeout which triggers the writing to the pipe
    DefaultEventDispatcherTypes::IdleEventSource idleSource(sourceManager, [&]() {
                static int i = 0;
                i++;
                log_debug() << "idle called " << i << " times";

                usleep(100000);

                return IdleEventSource::ReportStatus::KEEP_ENABLED;
            });
    idleSource.enable();

    DefaultEventDispatcherTypes::TimeOutEventSource timeOutStopIdle(sourceManager, [&]() {
                idleSource.disable();
                log_debug() << "Idle source disabled";
                return TimeOutEventSource::ReportStatus::DISABLE;
            }, 2000);
    timeOutStopIdle.enable();

    DefaultEventDispatcherTypes::ChannelWatchEventSource pipeInputSource(sourceManager, [&](
                ChannelWatchEventSource::Event e) {

                char bytes[64];

                auto r = read(pipeIn, bytes, sizeof(bytes));
                if (r < 0) {
                    log_error() << "Can not read data";
                    mainLoop.quit();
                } else {
                    log_debug() << "Read " << r << " bytes from pipe";
                }

                return ChannelWatchEventSource::ReportStatus::KEEP_ENABLED;

            }, pipeIn, ChannelWatchEventSource::Event::READ_AVAILABLE);
    pipeInputSource.enable();

    DefaultEventDispatcherTypes::ChannelWatchEventSource hangUpSource(sourceManager, [&](
                ChannelWatchEventSource::Event e) {
                log_debug() << "Hang up detected => exit main loop";
                mainLoop.quit();
                return ChannelWatchEventSource::ReportStatus::DISABLE;
            }, pipeIn, ChannelWatchEventSource::Event::HANG_UP);
    hangUpSource.enable();

    my_c_library_init_function(&mainLoop.getSourceManager());

    // Run the main loop. The method will return after the quit() method has been called
    mainLoop.run();

    log_debug() << "Terminated";

    return 0;
}
