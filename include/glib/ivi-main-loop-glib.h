    /* Copyright (C) 2015 Jacques Guillou */
    /* Copyright (C) 2015 Pelagicore AB  <info@pelagicore.com> */

    /* This library is free software; you can redistribute it and/or */
    /* modify it under the terms of the GNU Library General Public */
    /* License as published by the Free Software Foundation; either */
    /* version 2 of the License, or (at your option) any later version. */

    /* This library is distributed in the hope that it will be useful, */
    /* but WITHOUT ANY WARRANTY; without even the implied warranty of */
    /* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
    /* Library General Public License for more details. */

    /* A copy of the GNU Library General Public License is included in the */
    /* "LICENSE" file.  */

#pragma once

#include "ivi-main-loop-common.h"
#include "glib.h"

namespace ivi {

static constexpr int UNREGISTERED_SOURCE = -1;

class GLibEventDispatcher;

class GLibIdleEventSource :
    public IdleEventSource
{

public:
    GLibIdleEventSource(GLibEventDispatcher &mainLoop, CallBackFunction callBackFunction) :
        IdleEventSource(callBackFunction),
        m_mainLoop(mainLoop)
    {
        m_func = callBackFunction;
    }

    ~GLibIdleEventSource();

    void enable() override;

    void disable() override;

private:
    static gboolean onGLibCallback(gpointer data);

    GSource *m_source = nullptr;
    GLibEventDispatcher &m_mainLoop;

};


class GLibTimeOutEventSource :
    public TimeOutEventSource
{
public:
    GLibTimeOutEventSource(GLibEventDispatcher &mainLoop, CallBackFunction callBackFunction, DurationInMilliseconds duration) :
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

    GLibEventDispatcher &m_mainLoop;
    GSource *m_source = nullptr;
};

class GLibChannelWatchEventSource :
    public ChannelWatchEventSource
{
public:
    GLibChannelWatchEventSource(GLibEventDispatcher &mainLoop, CallBackFunction callBackFunction, FileDescriptor fileDescriptor,
                Event events);

    ~GLibChannelWatchEventSource();

    void disable() override;

    void enable() override;

private:
    static gboolean onSocketDataAvailableGLibCallback(GIOChannel *gio, GIOCondition condition, gpointer data);

    static Event toEventSource(const GIOCondition condition);

    static GIOCondition toGIOCondition(const Event event);

    bool m_isEnabled = false;
    gint inputSourceID = UNREGISTERED_SOURCE;

    GIOChannel *m_channel = nullptr;
    GLibEventDispatcher &m_mainLoop;
    Event m_events;

};

/**
 * That class implements the EventDispatcher interface using glib's main loop functions
 */
class GLibEventDispatcher :
    public EventDispatcher
{
public:
    typedef GLibIdleEventSource IdleEventSourceType;
    typedef GLibTimeOutEventSource TimeOutEventSourceType;
    typedef GLibChannelWatchEventSource FileDescriptorWatchEventSourceType;

    /**
     * Construct an instance using GLib's default main context if we do not have any instance of GLibEventDispatcher using that
     * context yet, otherwise we create a dedicated context
     */
    GLibEventDispatcher();

    /**
     * Construct a glib event dispatcher using the given context
     */
    GLibEventDispatcher(GMainContext* context);

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
