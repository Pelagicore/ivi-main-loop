/*
 *   Copyright (C) 2015 Pelagicore AB
 *   All rights reserved.
 */
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
