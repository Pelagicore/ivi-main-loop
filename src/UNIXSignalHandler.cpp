#include <assert.h>
#include "unistd.h"

#ifdef __linux__
#define USE_SIGNALFD_
#include <sys/signalfd.h>
#endif

#include "ivi-main-loop-unix-signal.h"
#include "ivi-main-loop-log.h"

//#undef USE_SIGNALFD_

namespace ivi_main_loop {
#ifndef USE_SIGNALFD_
static int s_UNIXSignalsPipe[2];

static void sSignalReceived(int signal)
{
    write(s_UNIXSignalsPipe[1], &signal, sizeof(signal));
}
#endif

/**
 * Return
 */
static bool isRunningInDebugger()
{
    // If we run in a debugger, we need to work around the problem that GDB forwards the SIGINT to our application
    // when it is asked to interrupt the execution, which causes our application to shutdown
    // TODO : find a generic way to detect the debugger
    return (getenv("EclipseVersion") != nullptr);     // If we detect Eclipse, we are probably debugging...
}

UNIXSignalHandler::UNIXSignalHandler(ivi_main_loop::EventSourceManager &dispatcher, const HandlerMap &handlers)
    : m_dispatcher(dispatcher)
{
    addHandlers(handlers);
}

void UNIXSignalHandler::disable()
{
    if (m_eventSource) {
        m_eventSource->disable();
    }
}

void UNIXSignalHandler::enable()
{
#ifdef USE_SIGNALFD_
    sigset_t mask;
    sigemptyset(&mask);

    for (auto sig : m_handlers) {
        if ((sig.first != SIGINT) || (!isRunningInDebugger())) {
            sigaddset(&mask, sig.first);
        }
    }

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        log_error() << "Error during sigprocmask() call";
    }

    unixSignalFileDescriptor = signalfd(-1, &mask, 0);

    if (unixSignalFileDescriptor == -1) {
        log_error() << "Error during signalfd() call";
    }
#else

    if (s_UNIXSignalsPipe[0] != 0) {
        log_error() << "Only one signal handler can be installed, unless signalfd() is used";
    }

    int returnCode = pipe(s_UNIXSignalsPipe);
    assert(returnCode == 0);
    unixSignalFileDescriptor = s_UNIXSignalsPipe[0];

    for (auto sig : m_handlers) {
        if ((sig.first != SIGINT) || (!isRunningInDebugger())) {
            signal(sig.first, sSignalReceived);
        }
    }

#endif

    m_eventSource.reset(m_dispatcher.newChannelWatchEventSource([this](ivi_main_loop::ChannelWatchEventSource::Event event) {
#ifdef USE_SIGNALFD_
                        struct signalfd_siginfo fdsi;

                        int s = read(unixSignalFileDescriptor, &fdsi, sizeof(struct signalfd_siginfo));

                        if (s != sizeof(struct signalfd_siginfo)) {
                            log_error() << "Error reading signal value";
                        }

                        int signal = fdsi.ssi_signo;
#else
                        int signal;
                        read(unixSignalFileDescriptor, &signal, sizeof(signal));
#endif

                        // Call handler
                        assert(m_handlers.count(signal) != 0);
                        m_handlers[signal](signal);

                        return ivi_main_loop::ChannelWatchEventSource::ReportStatus::KEEP_ENABLED;
                    }, unixSignalFileDescriptor, ivi_main_loop::ChannelWatchEventSource::Event::READ_AVAILABLE));

    m_eventSource->enable();
}

void UNIXSignalHandler::addHandlers(const HandlerMap &handlers)
{
    for (auto sig : handlers) {
        m_handlers.insert(sig);
    }
}
}
