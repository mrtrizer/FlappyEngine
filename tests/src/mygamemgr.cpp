#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "mygamemgr.h"
#include <core/flappyapp.h>

namespace game {

using namespace std;
using namespace flappy;

void MyGameMgr::init() {
    Catch::Session session;
    session.configData().showDurations = Catch::ShowDurations::Always;
    auto args = flappyApp().lock()->args();
    if (session.applyCommandLine(args.size(), args.data()) == 0)
        session.run();
    else
        throw runtime_error("Catch command line error.");
}

} // flappy
