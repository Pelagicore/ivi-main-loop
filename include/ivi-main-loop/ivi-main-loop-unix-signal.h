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

#include <map>
#include <functional>
#include <memory>

#include "ivi-main-loop/ivi-main-loop.h"

namespace ivi_main_loop {

/**
 * That class lets you easily handle UNIX signals from your ivi main loop.
 */
class UNIXSignalHandler
{

public:
    typedef std::function<void (int)> HandlerFunction;
    typedef std::map<int, HandlerFunction> HandlerMap;

    UNIXSignalHandler(ivi_main_loop::EventSourceManager &dispatcher, const HandlerMap &handlers = HandlerMap());

    /**
     * @param handlers: contains the handlers for the various signals to handle
     */
    void addHandlers(const HandlerMap &handlers);

    void enable();

    void disable();

private:
    int unixSignalFileDescriptor = -1;
    std::unique_ptr<ivi_main_loop::ChannelWatchEventSource> m_eventSource;
    ivi_main_loop::EventSourceManager &m_dispatcher;
    std::map<int, HandlerFunction> m_handlers;

};

}
