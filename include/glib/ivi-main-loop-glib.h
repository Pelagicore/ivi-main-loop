#pragma once

#include "ivi-main-loop-common.h"
#include "glib.h"

namespace ivi {

static constexpr int UNREGISTERED_SOURCE = -1;

class GlibEventDispatcher;

class GLibIdle :
    public IdleEventSource
{

public:
    GLibIdle(GlibEventDispatcher &mainLoop, CallBackFunction callBackFunction) :
        IdleEventSource(callBackFunction),
        m_mainLoop(mainLoop)
    {
        m_func = callBackFunction;
    }

    ~GLibIdle();

    void enable() override;

    void disable() override;

private:
    static gboolean onGlibCallback(gpointer data);

    GSource *m_source = nullptr;
    GlibEventDispatcher &m_mainLoop;

};


class GLibTimeOut :
    public TimeOutEventSource
{
public:
    GLibTimeOut(GlibEventDispatcher &mainLoop, CallBackFunction callBackFunction, DurationInMilliseconds duration) :
        TimeOutEventSource(duration, callBackFunction), m_mainLoop(mainLoop)
    {
    }

public:
    ~GLibTimeOut();

    void setDuration(DurationInMilliseconds duration) override;

    void enable() override;

    void disable() override;

private:
    static gboolean onTimerCallback(gpointer data);

    GlibEventDispatcher &m_mainLoop;
    GSource *m_source = nullptr;
};

class GLibFileDescriptorWatch :
    public FileDescriptorWatchEventSource
{
public:
    GLibFileDescriptorWatch(GlibEventDispatcher &mainLoop, CallBackFunction callBackFunction, FileDescriptor fileDescriptor,
                Event events);

    ~GLibFileDescriptorWatch();

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
 * That class implements the MainLoopInterface using glib's main loop functions
 */
class GlibEventDispatcher :
    public EventDispatcher
{
public:
    typedef GLibIdle IdleEventSourceType;
    typedef GLibTimeOut TimoutEventSourceType;
    typedef GLibFileDescriptorWatch FileDescriptorWatchEventSourceType;

    GlibEventDispatcher(GMainContext *context = nullptr);

    IdleEventSource *newIdleEventSource(const IdleEventSource::CallBackFunction &callBackFunction) final override;

    TimeOutEventSource *newTimeoutEventSource(const TimeOutEventSource::CallBackFunction &callBackFunction,
                DurationInMilliseconds duration) final override;

    FileDescriptorWatchEventSource *newFileDescriptorWatchEventSource(
                const FileDescriptorWatchEventSource::CallBackFunction &callBackFunction, FileDescriptor filedescriptor,
                FileDescriptorWatchEventSource::Event events) final override;

    void run() final override;

    void quit() final override;

    GMainContext *getGMainContext()
    {
        return m_context;
    }

private:
    GMainContext *m_context = nullptr;
    GMainLoop *m_mainLoop = nullptr;

};

}
