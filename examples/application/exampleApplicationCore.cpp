/**
 * Basic example using the application core class
 */

#include "ivi-main-loop-app.h"
#include "ivi-main-loop-log.h"

using namespace ivi_main_loop;

int main(int argc, const char * *argv)
{
    static constexpr int TIMEOUT = 5000;

    ApplicationCore app;
    log_debug() << "Entering main for " << TIMEOUT << " seconds";
    app.run(TIMEOUT);
    log_debug() << "Terminated";

    return 0;
}
