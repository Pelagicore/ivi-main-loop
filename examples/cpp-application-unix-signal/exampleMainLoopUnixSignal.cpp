/**
 * Basic example using the main loop abstraction to listen to UNIX signals.
 */

#include "ivi-main-loop/ivi-main-loop-unix-signal.h"
#include "ivi-main-loop/ivi-main-loop.h"
#include "ivi-main-loop-log.h"

using namespace ivi_main_loop;

int main(int argc, const char * *argv)
{
    DefaultEventDispatcherTypes::EventDispatcher mainLoop;

    // Register UNIX signal handler
    auto signalHandler = [&](int signal) {
        log_debug() << "Received signal : " << signal;
        mainLoop.quit();
    };

    UNIXSignalHandler handler(mainLoop.getSourceManager(),
            UNIXSignalHandler::HandlerMap {{SIGINT, signalHandler}, {SIGTERM, signalHandler}});
    handler.enable();

    // Run the main loop. The method will return after the quit() method has been called
    mainLoop.run();

    log_debug() << "Terminated";

    return 0;
}
