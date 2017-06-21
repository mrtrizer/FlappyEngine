#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include <exception>

using namespace std;

int main(int argc, char *argv[])
{
    Catch::Session session;
    session.configData().showDurations = Catch::ShowDurations::Always;

    if (session.applyCommandLine(argc, argv) == 0)
        session.run();
    else
        throw runtime_error("Catch command line error.");

    return 0;
}
