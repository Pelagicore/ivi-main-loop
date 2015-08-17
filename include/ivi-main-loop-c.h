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

#include "ivi-main-loop-c-types.h"

typedef struct
{
    /// The function to be called
    IVIMainLoop_EventSource_ReportStatus (*function)(IVIMainLoop_ChannelWatchEventSource_Event event, const void *data);

    /// Some user data to be passed to the callback function
    const void *data;
} IVIMainLoop_ChannelWatch_CallBack;


typedef struct
{
    /// The function to be called
    IVIMainLoop_EventSource_ReportStatus (*function)(const void *data);

    /// Some user data to be passed to the callback function
    const void *data;
} IVIMainLoop_TimeOut_CallBack;

typedef struct
{
    /// The function to be called
    IVIMainLoop_EventSource_ReportStatus (*function)(const void *data);

    /// Some user data to be passed to the callback function
    const void *data;
} IVIMainLoop_Idle_CallBack;


#ifdef __cplusplus
extern "C" {
#endif

IVIMainLoop_EventDispatcher *ivi_main_loop_event_dispatcher_new();

void ivi_main_loop_event_dispatcher_run(IVIMainLoop_EventDispatcher *dispatcher);

void ivi_main_loop_event_dispatcher_quit(IVIMainLoop_EventDispatcher *dispatcher);

IVIMainLoop_EventSourceManager *ivi_main_loop_event_dispatcher_get_source_manager(IVIMainLoop_EventDispatcher *dispatcher);

IVIMainLoop_TimeOutEventSource *ivi_main_loop_timeout_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_TimeOut_CallBack callback,
        IVIMainLoop_DurationInMilliseconds duration);

IVIMainLoop_IdleEventSource *ivi_main_loop_idle_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_Idle_CallBack callback);

IVIMainLoop_ChannelWatchEventSource *ivi_main_loop_channel_watch_source_new(IVIMainLoop_EventSourceManager *sourceManager,
        IVIMainLoop_ChannelWatch_CallBack callback, IVIMainLoop_FileDescriptor fileDescriptor,
        IVIMainLoop_ChannelWatchEventSource_Event events);

void ivi_main_loop_timeout_source_enable(IVIMainLoop_TimeOutEventSource *source);
void ivi_main_loop_idle_source_enable(IVIMainLoop_IdleEventSource *source);
void ivi_main_loop_channel_watch_source_enable(IVIMainLoop_ChannelWatchEventSource *source);

void ivi_main_loop_timeout_source_disable(IVIMainLoop_TimeOutEventSource *source);
void ivi_main_loop_idle_source_disable(IVIMainLoop_IdleEventSource *source);
void ivi_main_loop_channel_watch_source_disable(IVIMainLoop_ChannelWatchEventSource *source);

#ifdef __cplusplus
}
#endif
