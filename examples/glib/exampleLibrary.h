#pragma once

#include "ivi-main-loop.h"
#include "ivi-main-loop-log.h"

/**
 * Example of a GENIVI library function which registers a new source in the given dispatcher.
 * GENIVI code should normally use the DefaultEventDispatcherTypes type, which is a typedef to either
 * a GLibEventDispatcher, or other GLib-free class
 */
inline void typicalGeniviLibraryFunction(ivi::DefaultEventDispatcherTypes::EventSourceManager &dispatcher)
{
    auto src = new ivi::DefaultEventDispatcherTypes::TimeOutEventSource(dispatcher, [&] () {
                log_debug() << "Timeout libraryFunction";
                return ivi::TimeOutEventSource::ReportStatus::KEEP_ENABLED;
            }, 2000);
    src->enable();
}

inline gboolean glibCallbackLib(gpointer data)
{
    log_debug() << "glibCallbackLib called";
    return true;
}

/**
 * Example of a library function which only accepts a GLib kind of mainloop reference.
 * That would typically be the case if the function was using some OSS software which only support GLib as
 * a main loop (e.g. PulseAudio glib main loop integration)
 */
inline void libraryFunctionGLibOnly(ivi::GLibEventSourceManager &dispatcher)
{
    // Get the GLib GMainContext reference
    GMainContext *context = dispatcher.getGMainContext();

    // Create our source using the GLib API directly
    GSource *glibSource = g_timeout_source_new(1800);
    g_source_set_callback(glibSource, glibCallbackLib, nullptr, nullptr);
    g_source_attach(glibSource, context);

}
