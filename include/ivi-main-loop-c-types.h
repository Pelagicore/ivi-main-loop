#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef int IVIMainLoop_DurationInMilliseconds;
typedef int IVIMainLoop_FileDescriptor;

/// Opaque structure representing an Event dispatcher. See ivi_main_loop::EventDispatcher
typedef struct
{
} IVIMainLoop_EventDispatcher;

/// Opaque structure representing an Event source manager. See ivi_main_loop::EventSourceManager
typedef struct
{
} IVIMainLoop_EventSourceManager;

/// Opaque structure representing a timeout event source. See ivi_main_loop::TimeOutEventSource
typedef struct
{
} IVIMainLoop_TimeOutEventSource;

/// Opaque structure representing an idle event source. See ivi_main_loop::IdleEventSource
typedef struct
{
} IVIMainLoop_IdleEventSource;

/// Opaque structure representing a channel watch event source. See ivi_main_loop::ChannelWatchEventSource
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
    /// The function to be called
    IVIMainLoop_EventSource_ReportStatus (*function)(const void *data);

    /// Some user data to be passed to the calback function
    const void *data;
} IVIMainLoop_TimeOut_CallBack;

typedef struct
{
    /// The function to be called
    IVIMainLoop_EventSource_ReportStatus (*function)(const void *data);

    /// Some user data to be passed to the callback function
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
    /// The function to be called
    IVIMainLoop_EventSource_ReportStatus (*function)(IVIMainLoop_ChannelWatchEventSource_Event event, const void *data);

    /// Some user data to be passed to the callback function
    const void *data;
} IVIMainLoop_ChannelWatch_CallBack;

#ifdef __cplusplus
}
#endif
