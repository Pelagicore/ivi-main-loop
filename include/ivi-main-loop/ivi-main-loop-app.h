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

#include "ivi-main-loop.h"
#include "ivi-main-loop-unix-signal.h"

#include <limits>

namespace ivi_main_loop {
/**
 */
class ApplicationCore
{
public:
    ApplicationCore() :
        m_signalHandler(m_eventDispatcher.getSourceManager(),
                UNIXSignalHandler::HandlerMap {{SIGINT,
                                                std::bind(&ApplicationCore::processAbortSignal, this,
                            std::placeholders::_1)},
                                               {SIGTERM,
                                                std::bind(&ApplicationCore::processAbortSignal, this, std::placeholders::_1)}}) {
    }

    ~ApplicationCore()
    {
    }

    /**
     * Run the main loop until either the given timeout is reached, or a termination signal is received
     */
    void run(DurationInMilliseconds timeoutInMilliseconds = 0)
    {
        DefaultEventDispatcherTypes::TimeOutEventSource timeoutSource(m_eventDispatcher.getSourceManager(), [&]() {
                        m_eventDispatcher.quit();
                        return TimeOutEventSource::ReportStatus::DISABLE;
                    }, timeoutInMilliseconds);

        if (timeoutInMilliseconds != 0) {
            timeoutSource.enable();
        }

        m_signalHandler.enable();

        m_eventDispatcher.run();
    }

    void quit()
    {
        m_eventDispatcher.quit();
    }


    DefaultEventDispatcherTypes::EventDispatcher &getEventDispatcher()
    {
        return m_eventDispatcher;
    }

private:
    /**
     *  Signal handler
     */
    void processAbortSignal(int sig)
    {
        m_eventDispatcher.quit();
    }

    DefaultEventDispatcherTypes::EventDispatcher m_eventDispatcher;
    UNIXSignalHandler m_signalHandler;
};
}
