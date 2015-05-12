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
