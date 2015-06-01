/**
 * Basic example using the main loop abstraction from a C application.
 * That example creates a pipe where bytes are regularly written and read, using the various kind of event sources available.
 * An idle event source is also used to be notified whenever nothing has to be done.
 */

#include "ivi-main-loop-c.h"

#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"

#include "../c-library/c-library.h"

IVIMainLoop_EventDispatcher *dispatcher;

int pipeIn;
int pipeOut;


void fd_set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

IVIMainLoop_EventSource_ReportStatus callbackTimeout(const void *data)
{
    char bytes[64] = {};
    int n = write(pipeOut, bytes, sizeof(bytes));
    if (n == sizeof(bytes)) {
        printf("Written %d to pipe\n", n);
    } else {
        printf("Can not write data to pipe\n");
        ivi_main_loop_event_dispatcher_quit(dispatcher);
    }

    printf("callbackFunction data = %d\n", *((int *) data));
    return IVI_MAIN_LOOP_KEEP_ENABLED;
}

IVIMainLoop_EventSource_ReportStatus callbackTimeout2(const void *data)
{
    static int i = 0;
    if (i++ > 10) {
        printf("Stopping main loop\n");
        ivi_main_loop_event_dispatcher_quit(dispatcher);
    }

    printf("callbackFunction2 data = %d\n", *((int *) data));
    return IVI_MAIN_LOOP_KEEP_ENABLED;
}

IVIMainLoop_EventSource_ReportStatus callbackFunctionIdle(const void *data)
{
    printf("callbackFunctionIdle data = %d\n", *((int *) data));
    usleep(200000);
    return IVI_MAIN_LOOP_KEEP_ENABLED;
}

IVIMainLoop_EventSource_ReportStatus callbackFunctionChannelInput(IVIMainLoop_ChannelWatchEventSource_Event event,
        const void *data)
{
    printf("callbackFunctionInput event:%d data = %d\n", (int)event, *((int *) data));

    char bytes[24];

    auto r = read(pipeIn, bytes, sizeof(bytes));
    if (r < 0) {
        printf("Can not read data\n");
        ivi_main_loop_event_dispatcher_quit(dispatcher);
    } else {
        printf("Read %d bytes from pipe\n", r);
    }

    return IVI_MAIN_LOOP_KEEP_ENABLED;
}

const int anInt = 5;
const int anInt2 = 3;
const int anInt3 = 7;
const int anInt4 = 17;

int main(int argc, const char * *argv)
{
    int pipes[2];
    if (pipe(pipes) != 0) {
        printf("Can not create pipe\n");
        exit(-1);
    }

    pipeIn = pipes[0];
    pipeOut = pipes[1];
    fd_set_non_blocking(pipeIn);
    fd_set_non_blocking(pipeOut);

    dispatcher = ivi_main_loop_event_dispatcher_new();
    IVIMainLoop_EventSourceManager *sourceManager = ivi_main_loop_event_dispatcher_get_source_manager(dispatcher);

    IVIMainLoop_TimeOut_CallBack callback = {.function = callbackTimeout, .data = &anInt};
    IVIMainLoop_TimeOutEventSource *source1 = ivi_main_loop_timeout_source_new(sourceManager, callback, 1000);
    ivi_main_loop_timeout_source_enable(source1);

    IVIMainLoop_TimeOut_CallBack callback2 = {.function = callbackTimeout2, .data = &anInt2};
    IVIMainLoop_TimeOutEventSource *source2 = ivi_main_loop_timeout_source_new(sourceManager, callback2, 800);
    ivi_main_loop_timeout_source_enable(source2);

    IVIMainLoop_Idle_CallBack callbackIdle = {.function = callbackFunctionIdle, .data = &anInt3};
    IVIMainLoop_IdleEventSource *sourceIdle = ivi_main_loop_idle_source_new(sourceManager, callbackIdle);
    ivi_main_loop_idle_source_enable(sourceIdle);

    IVIMainLoop_ChannelWatch_CallBack channelWatchCallBack = {.function = callbackFunctionChannelInput, .data = &anInt4};
    IVIMainLoop_ChannelWatchEventSource *sourceChannelWatchInput = ivi_main_loop_channel_watch_source_new(
            sourceManager, channelWatchCallBack, pipeIn, IVI_MAIN_LOOP_READ_AVAILABLE);
    ivi_main_loop_channel_watch_source_enable(sourceChannelWatchInput);

    my_c_library_init_function(sourceManager);

    ivi_main_loop_event_dispatcher_run(dispatcher);

    return 0;
}
