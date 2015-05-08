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
 * An idle event source can be used to be notified whenever no other source is triggered
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
 * A timeout source is after a certain amount of time.
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

    DurationInMilliseconds getDuration()
    {
        return m_duration;
    }

    virtual void setDuration(DurationInMilliseconds duration) = 0;

protected:
    DurationInMilliseconds m_duration;
    CallBackFunction m_func;

};

class FileDescriptorWatchEventSource :
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

    FileDescriptorWatchEventSource(const CallBackFunction &callBackFunction) :
        m_callBack(callBackFunction)
    {
    }

    virtual ~FileDescriptorWatchEventSource()
    {
    }

protected:
    CallBackFunction m_callBack;

};

class MainLoop
{
public:
    virtual ~MainLoop()
    {
    }

    virtual IdleEventSource *newIdleEventSource(const IdleEventSource::CallBackFunction &callBackFunction) = 0;

    virtual TimeOutEventSource *newTimeoutEventSource(const TimeOutEventSource::CallBackFunction &callBackFunction,
                DurationInMilliseconds duration) = 0;

    virtual FileDescriptorWatchEventSource *newFileDescriptorWatchEventSource(
                const FileDescriptorWatchEventSource::CallBackFunction &callBackFunction, FileDescriptor fileDescriptor,
                FileDescriptorWatchEventSource::Event events) = 0;

};

class EventDispatcher :
    public MainLoop
{
    virtual void run() = 0;

    virtual void quit() = 0;
};

}
