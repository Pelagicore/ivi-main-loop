#pragma once

#include <functional>

namespace ivi {

typedef int FileDescriptor;

/**
 * Any event source inherits from this class
 */
class EventSource
{
public:
    enum class ReportStatus
    {
        /// Returning that value from a source's callback function causes the source to remain enabled
        KEEP_ENABLED,

        /// Returning that value from a source's callback function causes the source to be disabled
        DISABLE
    };

    /**
     * Enable the source.
     */
    virtual void enable() = 0;

    /**
     * Disable the source
     */
    virtual void disable() = 0;

};

/**
 * An idle event source can be used to be notified whenever a dispatcher has no non-idle to trigger
 */
class IdleEventSource :
    public EventSource
{
public:
    typedef std::function<ReportStatus()> CallBackFunction;

    IdleEventSource(const CallBackFunction &function) :
        m_func(function)
    {
    }

    virtual ~IdleEventSource()
    {
    }

protected:
    CallBackFunction m_func;

};

typedef int DurationInMilliseconds;

/**
 * A timeout source is triggered after a certain amount of time.
 */
class TimeOutEventSource :
    public EventSource
{
public:
    typedef std::function<ReportStatus()> CallBackFunction;

    TimeOutEventSource(DurationInMilliseconds duration, const CallBackFunction &callBackFunction) :
        m_duration(duration), m_func(callBackFunction)
    {
    }

    virtual ~TimeOutEventSource()
    {
    }

    DurationInMilliseconds getDuration() const
    {
        return m_duration;
    }

    virtual void setDuration(DurationInMilliseconds duration) = 0;

protected:
    DurationInMilliseconds m_duration;
    CallBackFunction m_func;

};

/**
 * This kind of source can be used to be notified whenever an event has occurred concerning a channel.
 * The channel is typically identified by a file descriptor.
 */
class ChannelWatchEventSource :
    public EventSource
{
public:
    enum class Event
    {
        /// No event occurred
        NONE,

        /// Some data is available from the channel
        READ_AVAILABLE = 1,

        /// Some data can be written to the channel without blocking
        WRITE_AVAILABLE = 2,

        /// The channel has been close, which means no data can be read from the corresponding file descriptor
        HANG_UP = 4
    };

    typedef std::function<ReportStatus(Event)> CallBackFunction;

    ChannelWatchEventSource(const CallBackFunction &callBackFunction) :
        m_callBack(callBackFunction)
    {
    }

    virtual ~ChannelWatchEventSource()
    {
    }

protected:
    CallBackFunction m_callBack;

};

/**
 * That interface is used to create new sources attached to a dispatcher
 */
class EventSourceFactory
{
public:
    virtual ~EventSourceFactory()
    {
    }

    /**
     * Create a new idle event source.
     */
    virtual IdleEventSource *newIdleEventSource(const IdleEventSource::CallBackFunction &callBackFunction) = 0;

    virtual TimeOutEventSource *newTimeoutEventSource(const TimeOutEventSource::CallBackFunction &callBackFunction,
                DurationInMilliseconds duration) = 0;

    virtual ChannelWatchEventSource *newFileDescriptorWatchEventSource(
                const ChannelWatchEventSource::CallBackFunction &callBackFunction, FileDescriptor fileDescriptor,
				ChannelWatchEventSource::Event events) = 0;

};

/**
 * Event dispatcher interface. An event dispatcher implements a main loop is able to handle various event sources.
 */
class EventDispatcher :
    public EventSourceFactory
{
    /**
     * Runs the main loop. That method does not return until the quit() method has been called.
     */
    virtual void run() = 0;

    /**
     * Stops the main loop. Calling that method causes the main loop to stop handling any event and ensures that the run() method exits as soon as possible.
     */
    virtual void quit() = 0;

};

}
