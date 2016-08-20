#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "mygamemgr.h"

namespace game {

using namespace std;
using namespace flappy;

void MyGameMgr::init() {
    Catch::Session session; // There must be exactly once instance
    char* argv[1];
    if (session.applyCommandLine(1, argv) == 0)
        session.run();
    else
        throw runtime_error("Catch command line error.");
}

} // flappy
