#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef int IVIMainLoop_DurationInMilliseconds;
typedef int IVIMainLoop_FileDescriptor;

/// Opaque structure representing an Event dispatcher. See ivi::EventDispatcher
typedef struct
{
} IVIMainLoop_EventDispatcher;

/// Opaque structure representing an Event source manager. See ivi::EventSourceManager
typedef struct
{
} IVIMainLoop_EventSourceManager;

/// Opaque structure representing a timeout event source. See ivi::TimeoutEventSource
typedef struct
{
} IVIMainLoop_TimeOutEventSource;

/// Opaque structure representing an idle event source. See ivi::IdleEventSource
typedef struct
{
} IVIMainLoop_IdleEventSource;

/// Opaque structure representing a channel watch event source. See ivi::IdleEventSource
typedef struct
{
} IVIMainLoop_ChannelWatchEventSource;


typedef enum {
    /// Returning that value from a source's callback function causes the source to remain enabled
    IVI_MAIN_LOOP_KEEP_ENABLED,

    /// Returning that value from a source's callback function causes the source to be disabled
    IVI_MAIN_LOOP_DISABLE
} IVIMainLoop_EventSource_ReportStatus;


typedef struct
{
    IVIMainLoop_EventSource_ReportStatus (*function)(const void *data);
    const void *data;
} IVIMainLoop_TimeOut_CallBack;

typedef struct
{
    IVIMainLoop_EventSource_ReportStatus (*function)(const void *data);
    const void *data;
} IVIMainLoop_Idle_CallBack;


typedef enum {
    /// No event occurred
    IVI_MAIN_LOOP_NONE,

    /// Some data is available from the channel
    IVI_MAIN_LOOP_READ_AVAILABLE = 1,

    /// Some data can be written to the channel without blocking
    IVI_MAIN_LOOP_WRITE_AVAILABLE = 2,

    /// The channel has been closed, which means no data can be read from the corresponding file descriptor
    IVI_MAIN_LOOP_HANG_UP = 4
} IVIMainLoop_ChannelWatchEventSource_Event;


typedef struct
{
    IVIMainLoop_EventSource_ReportStatus (*function)(IVIMainLoop_ChannelWatchEventSource_Event event, const void *data);
    const void *data;
} IVIMainLoop_ChannelWatch_CallBack;


IVIMainLoop_EventDispatcher *ivi_main_loop_event_dispatcher_new();

void ivi_main_loop_event_dispatcher_run(IVIMainLoop_EventDispatcher *dispatcher);

void ivi_main_loop_event_dispatcher_quit(IVIMainLoop_EventDispatcher *dispatcher);

IVIMainLoop_EventSourceManager *ivi_main_loop_event_dispatcher_get_source_manager(IVIMainLoop_EventDispatcher *dispatcher);

IVIMainLoop_TimeOutEventSource *ivi_main_loop_timeout_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_TimeOut_CallBack callback,
        IVIMainLoop_DurationInMilliseconds duration);

void ivi_main_loop_timeout_source_enable(IVIMainLoop_TimeOutEventSource *source);
void ivi_main_loop_idle_source_enable(IVIMainLoop_IdleEventSource *source);
void ivi_main_loop_channel_watch_source_enable(IVIMainLoop_ChannelWatchEventSource *source);

void ivi_main_loop_timeout_source_disable(IVIMainLoop_TimeOutEventSource *source);
void ivi_main_loop_idle_source_disable(IVIMainLoop_IdleEventSource *source);
void ivi_main_loop_channel_watch_source_disable(IVIMainLoop_ChannelWatchEventSource *source);

IVIMainLoop_IdleEventSource *ivi_main_loop_idle_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_Idle_CallBack callback);

#ifdef __cplusplus
}
#endif
