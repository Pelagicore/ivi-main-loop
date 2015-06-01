/**
 * Example of a typical library providing a C-interface
 */

#pragma once

#include "ivi-main-loop-c.h"

#ifdef __cplusplus
extern "C" {
#endif

static IVIMainLoop_EventSource_ReportStatus callbackMyCLibrary(const void *data)
{
    printf("callbackMyCLibrary\n");
    return IVI_MAIN_LOOP_KEEP_ENABLED;
}

/**
 * Initialize the library using the given source manager object where the library is going to add its event sources
 */
inline void my_c_library_init_function(IVIMainLoop_EventSourceManager *sourceManager)
{
    IVIMainLoop_TimeOut_CallBack callback = {.function = callbackMyCLibrary, .data = NULL};
    IVIMainLoop_TimeOutEventSource *source = ivi_main_loop_timeout_source_new(sourceManager, callback, 300);
    ivi_main_loop_timeout_source_enable(source);
}

#ifdef __cplusplus
}
#endif
