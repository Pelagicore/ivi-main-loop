#pragma once

#include "ivi-main-loop-c-types.h"

#ifdef __cplusplus
extern "C" {
#endif

IVIMainLoop_EventDispatcher *ivi_main_loop_event_dispatcher_new();

void ivi_main_loop_event_dispatcher_run(IVIMainLoop_EventDispatcher *dispatcher);

IVIMainLoop_EventSourceManager *ivi_main_loop_event_dispatcher_get_source_manager(IVIMainLoop_EventDispatcher *dispatcher);

IVIMainLoop_TimeOutEventSource *ivi_main_loop_timeout_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_TimeOut_CallBack callback,
        IVIMainLoop_DurationInMilliseconds duration);

void ivi_main_loop_timeout_source_enable(IVIMainLoop_TimeOutEventSource *source);

IVIMainLoop_IdleEventSource *ivi_main_loop_idle_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_Idle_CallBack callback);

IVIMainLoop_ChannelWatchEventSource *ivi_main_loop_channel_watch_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_ChannelWatch_CallBack callback, IVIMainLoop_FileDescriptor fileDescriptor,
        IVIMainLoop_ChannelWatchEventSource_Event events);

#ifdef __cplusplus
}
#endif
