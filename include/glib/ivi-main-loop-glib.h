#pragma once

#include "ivi-main-loop-common.h"
#include "glib.h"

namespace ivi {

static constexpr int UNREGISTERED_SOURCE = -1;

class GlibEventDispatcher;

class GLibIdleEventSource :
    public IdleEventSource
{

public:
    GLibIdleEventSource(GlibEventDispatcher &mainLoop, CallBackFunction callBackFunction) :
        IdleEventSource(callBackFunction),
        m_mainLoop(mainLoop)
    {
        m_func = callBackFunction;
    }

    ~GLibIdleEventSource();

    void enable() override;

    void disable() override;

private:
    static gboolean onGlibCallback(gpointer data);

    GSource *m_source = nullptr;
    GlibEventDispatcher &m_mainLoop;

};


class GLibTimeOutEventSource :
    public TimeOutEventSource
{
public:
    GLibTimeOutEventSource(GlibEventDispatcher &mainLoop, CallBackFunction callBackFunction, DurationInMilliseconds duration) :
        TimeOutEventSource(duration, callBackFunction), m_mainLoop(mainLoop)
    {
    }

public:
    ~GLibTimeOutEventSource();

    void setDuration(DurationInMilliseconds duration) override;

    void enable() override;

    void disable() override;

private:
    static gboolean onTimerCallback(gpointer data);

    GlibEventDispatcher &m_mainLoop;
    GSource *m_source = nullptr;
};

class GLibChannelWatchEventSource :
    public ChannelWatchEventSource
{
public:
    GLibChannelWatchEventSource(GlibEventDispatcher &mainLoop, CallBackFunction callBackFunction, FileDescriptor fileDescriptor,
                Event events);

    ~GLibChannelWatchEventSource();

    void disable() override;

    void enable() override;

private:
    static gboolean onSocketDataAvailableGlibCallback(GIOChannel *gio, GIOCondition condition, gpointer data);

    static Event toEventSource(const GIOCondition condition);

    static GIOCondition toGIOCondition(const Event event);

    bool m_isEnabled = false;
    gint inputSourceID = UNREGISTERED_SOURCE;

    GIOChannel *m_channel = nullptr;
    GlibEventDispatcher &m_mainLoop;
    Event m_events;

};


/**
 * That class implements the EventDispatcher interface using glib's main loop functions
 */
class GlibEventDispatcher :
    public EventDispatcher
{
public:
    typedef GLibIdleEventSource IdleEventSourceType;
    typedef GLibTimeOutEventSource TimoutEventSourceType;
    typedef GLibChannelWatchEventSource FileDescriptorWatchEventSourceType;

    /**
     * Construct an instance using Glib's default main context if we do not have any instance of GlibEventDispatcher using that
     * context yet, otherwise we create a dedicated context
     */
    GlibEventDispatcher();

    IdleEventSource *newIdleEventSource(const IdleEventSource::CallBackFunction &callBackFunction) final override;

    TimeOutEventSource *newTimeOutEventSource(const TimeOutEventSource::CallBackFunction &callBackFunction,
                DurationInMilliseconds duration) final override;

    ChannelWatchEventSource *newChannelWatchEventSource(const ChannelWatchEventSource::CallBackFunction &callBackFunction,
                FileDescriptor filedescriptor,
                ChannelWatchEventSource::Event events) final override;

    void run() final override;

    void quit() final override;

    /**
     * Return the glib main context reference
     */
    GMainContext *getGMainContext()
    {
        return m_context;
    }

private:
    GMainContext *m_context = nullptr;
    GMainLoop *m_mainLoop = nullptr;

    static bool s_bDefaultContextAlreadyUsed;

};

}
