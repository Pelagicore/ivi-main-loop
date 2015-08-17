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

#include "ivi-main-loop/ivi-main-loop-common.h"
#include "glib.h"

namespace ivi_main_loop {

static constexpr int UNREGISTERED_SOURCE = -1;

class GLibEventSourceManager;

class GLibIdleEventSource :
    public IdleEventSource
{

public:
    GLibIdleEventSource(GLibEventSourceManager &mainLoop, CallBackFunction callBackFunction) :
        IdleEventSource(callBackFunction),
        m_mainLoop(mainLoop)
    {
        m_func = callBackFunction;
    }

    ~GLibIdleEventSource();

    void enable() override;

    void disable() override;

    bool isEnabled() const override;

private:
    static gboolean onGLibCallback(gpointer data);

    GSource *m_source = nullptr;
    GLibEventSourceManager &m_mainLoop;

};


class GLibTimeOutEventSource :
    public TimeOutEventSource
{
public:
    GLibTimeOutEventSource(GLibEventSourceManager &mainLoop, CallBackFunction callBackFunction,
                DurationInMilliseconds duration) :
        TimeOutEventSource(duration, callBackFunction), m_mainLoop(mainLoop)
    {
    }

public:
    ~GLibTimeOutEventSource();

    void enable() override;

    void disable() override;

    bool isEnabled() const override;

private:
    static gboolean onTimerCallback(gpointer data);

    GLibEventSourceManager &m_mainLoop;
    GSource *m_source = nullptr;
};

class GLibChannelWatchEventSource :
    public ChannelWatchEventSource
{
public:
    GLibChannelWatchEventSource(GLibEventSourceManager &mainLoop, CallBackFunction callBackFunction,
                FileDescriptor fileDescriptor,
                Event events);

    ~GLibChannelWatchEventSource();

    void disable() override;

    void enable() override;

    bool isEnabled() const
    {
        return (inputSourceID != UNREGISTERED_SOURCE);
    }

private:
    static gboolean onSocketDataAvailableGLibCallback(GIOChannel *gio, GIOCondition condition, gpointer data);

    static Event toEventSource(const GIOCondition condition);

    static GIOCondition toGIOCondition(const Event event);

    gint inputSourceID = UNREGISTERED_SOURCE;

    GIOChannel *m_channel = nullptr;
    GLibEventSourceManager &m_mainLoop;
    Event m_events;

};

class GLibEventDispatcher;

/**
 * That class implements the EventDispatcher interface using glib's main loop functions
 */
class GLibEventSourceManager :
    public EventSourceManager
{
public:
    GLibEventSourceManager(GMainContext *context = nullptr);

    GLibIdleEventSource *newIdleEventSource(const IdleEventSource::CallBackFunction &callBackFunction) final override;

    GLibTimeOutEventSource *newTimeOutEventSource(const TimeOutEventSource::CallBackFunction &callBackFunction,
                DurationInMilliseconds duration) final override;

    GLibChannelWatchEventSource *newChannelWatchEventSource(const ChannelWatchEventSource::CallBackFunction &callBackFunction,
                FileDescriptor filedescriptor,
                ChannelWatchEventSource::Event events) final override;

    /**
     * Return the glib main context reference
     */
    GMainContext *getGMainContext()
    {
        return m_context;
    }

private:
    friend GLibEventDispatcher;
    GMainContext *m_context = nullptr;

};

/**
 * That class implements the EventDispatcher interface using glib's main loop functions
 */
class GLibEventDispatcher :
    public EventDispatcher
{
public:
    /**
     * Construct an instance using GLib's default main context if we do not have any instance of GLibEventDispatcher using that
     * context yet, otherwise we create a dedicated context
     */
    GLibEventDispatcher();

    /**
     * Construct a glib event dispatcher using the given context
     */
    GLibEventDispatcher(GMainContext *context);

    GLibEventSourceManager &getSourceManager() override
    {
        return m_sourceFactory;
    }

    void run() final override;

    void quit() final override;

private:
    GMainLoop *m_mainLoop = nullptr;
    GLibEventSourceManager m_sourceFactory;

    static bool s_bDefaultContextAlreadyUsed;

};

/**
 * This class contains typedefs which user code can use to refer to the exact objects types instead of referring
 * only to the abstract interfaces. One of the benefits of referring to the exact data type is that a build-time failure
 * is generated in case of backend type incompatibility.
 * User code typically accesses those typedefs via the DefaultEventDispatcherTypes class.
 */
class GLibEventDispatcherTypes
{
public:
    typedef GLibIdleEventSource IdleEventSource;
    typedef GLibTimeOutEventSource TimeOutEventSource;
    typedef GLibChannelWatchEventSource ChannelWatchEventSource;
    typedef GLibEventDispatcher EventDispatcher;
    typedef GLibEventSourceManager EventSourceManager;
};

}
