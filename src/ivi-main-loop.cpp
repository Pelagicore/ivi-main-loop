#include "ivi-main-loop.h"
#include "ivi-main-loop-c.h"

using namespace ivi_main_loop;

IVIMainLoop_EventDispatcher *ivi_main_loop_event_dispatcher_new()
{
    return new DefaultEventDispatcherTypes::EventDispatcher();
}

void ivi_main_loop_event_dispatcher_run(IVIMainLoop_EventDispatcher *dispatcher)
{
    static_cast<EventDispatcher *>(dispatcher)->run();
}

void ivi_main_loop_event_dispatcher_quit(IVIMainLoop_EventDispatcher *dispatcher)
{
    static_cast<EventDispatcher *>(dispatcher)->quit();
}

void ivi_main_loop_timeout_source_enable(IVIMainLoop_TimeOutEventSource *source)
{
    static_cast<TimeOutEventSource *>(source)->enable();
}

void ivi_main_loop_idle_source_enable(IVIMainLoop_IdleEventSource *source)
{
    static_cast<IdleEventSource *>(source)->enable();
}

void ivi_main_loop_channel_watch_source_enable(IVIMainLoop_ChannelWatchEventSource *source)
{
    static_cast<ChannelWatchEventSource *>(source)->enable();
}

void ivi_main_loop_timeout_source_disable(IVIMainLoop_TimeOutEventSource *source)
{
    static_cast<TimeOutEventSource *>(source)->disable();
}

void ivi_main_loop_idle_source_disable(IVIMainLoop_IdleEventSource *source)
{
    static_cast<IdleEventSource *>(source)->disable();
}

void ivi_main_loop_channel_watch_source_disable(IVIMainLoop_ChannelWatchEventSource *source)
{
    static_cast<ChannelWatchEventSource *>(source)->disable();
}

IVIMainLoop_EventSourceManager *ivi_main_loop_event_dispatcher_get_source_manager(IVIMainLoop_EventDispatcher *dispatcher)
{
    auto &manager = static_cast<EventDispatcher *>(dispatcher)->getSourceManager();
    return &manager;
}

IVIMainLoop_TimeOutEventSource *ivi_main_loop_timeout_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_TimeOut_CallBack callback, DurationInMilliseconds duration)
{
    auto sourceManager_ = static_cast<EventSourceManager *>(sourceManager);
    std::function<IVIMainLoop_EventSource_ReportStatus()> f = std::bind(callback.function, callback.data);
    // TODO : check how to get rid of the following cast
    std::function<EventSource::ReportStatus()> *f2 = reinterpret_cast<std::function<EventSource::ReportStatus()> *>(&f);
    return sourceManager_->newTimeOutEventSource(*f2, duration);
}

IVIMainLoop_IdleEventSource *ivi_main_loop_idle_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_Idle_CallBack callback)
{
    auto sourceManager_ = static_cast<EventSourceManager *>(sourceManager);
    std::function<IVIMainLoop_EventSource_ReportStatus()> f = std::bind(callback.function, callback.data);
    // TODO : check how to get rid of the following cast
    std::function<EventSource::ReportStatus()> f2 = reinterpret_cast<std::function<EventSource::ReportStatus()> &>(f);
    return sourceManager_->newIdleEventSource(f2);
}

IVIMainLoop_ChannelWatchEventSource *ivi_main_loop_channel_watch_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_ChannelWatch_CallBack callback, IVIMainLoop_FileDescriptor fileDescriptor,
        IVIMainLoop_ChannelWatchEventSource_Event events)
{
    auto sourceManager_ = static_cast<EventSourceManager *>(sourceManager);
    std::function<IVIMainLoop_EventSource_ReportStatus(IVIMainLoop_ChannelWatchEventSource_Event)> f = std::bind(
            callback.function, std::placeholders::_1, callback.data);
    // TODO : check how to get rid of the following cast
    auto &f2 = reinterpret_cast<std::function<EventSource::ReportStatus(ChannelWatchEventSource::Event)> &>(f);
    return sourceManager_->newChannelWatchEventSource(f2, fileDescriptor, static_cast<ChannelWatchEventSource::Event>(events));
}
