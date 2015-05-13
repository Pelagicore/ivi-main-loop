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

#include <iostream>

#ifdef USE_IVI_LOGGING
#include "ivi-logging.h"
#else
#define log_debug() ivi::Log(std::cout).stream()
#define log_error() ivi::Log(std::cerr).stream()
#define LOG_DECLARE_CONTEXT
#endif

namespace ivi {

#ifdef USE_IVI_LOGGING
typedef logging::DefaultLogContext LogContext;
LOG_IMPORT_DEFAULT_CONTEXT(iviMainLoopContext);
#else

// Convenience class which automatically writes std::endl to the stream during destruction, for compatibility with ivi-logging
class Log
{
public:
    Log(std::ostream &s)
    {
        m_stream = &s;
    }

    ~Log()
    {
        *m_stream << std::endl;
    }

    std::ostream &stream()
    {
        return *m_stream;
    }

private:
    std::ostream *m_stream = nullptr;
};
#endif

}
