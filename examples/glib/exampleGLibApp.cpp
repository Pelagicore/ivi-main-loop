/**
 * This example shows how to provide a dispatcher reference to library functions, in an application which is based
 * on a glib main loop.
 * Note that a small variation of this example could be made if the actual main loop was based on Qt. It would just be a matter
 * of getting the GMainContext* reference from Qt, via some private Qt methods.
 */

#include <stdio.h>
#include <glib.h>

#include "ivi-main-loop/ivi-main-loop.h"
#include "ivi-main-loop-log.h"

#include "exampleLibrary.h"

using namespace ivi_main_loop;

gboolean glibCallback(gpointer data)
{
    log_debug() << "glib callback called";
    return TRUE;
}

int main(int argc, const char * *argv)
{
    // Create our context
    GMainContext *context = g_main_context_new();

    // Create a source using the glib API
    GSource *glibSource = g_timeout_source_new(1500);
    g_source_set_callback(glibSource, glibCallback, nullptr, nullptr);
    g_source_attach(glibSource, context);

    // Create our dispatcher object which wraps the glib context
    GLibEventDispatcher dispatcher(context);

    // Now we have a "GENIVI-compatible" dispatcher reference, which we can pass to GENIVI components, under certain conditions

    // Call a function which accepts a parameter of type DefaultEventDispatcherType. If the DefaultEventDispatcherType is not set to GLibEventDispatcher, this should cause a compile error
    typicalGeniviLibraryFunction(dispatcher.getSourceManager());

    // Call a function which accepts a parameter of type Glib. For sure, this call will not fail here
    libraryFunctionGLibOnly(dispatcher.getSourceManager());

    // Create the main loop object
    GMainLoop *main_loop = g_main_loop_new(context, FALSE);

    log_debug() << "entering main loop";
    g_main_loop_run(main_loop);

    return 0;
}
