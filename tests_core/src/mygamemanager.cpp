#define CATCH_CONFIG_RUNNER
#include "catch.h"

#include "mygamemanager.h"
#include <core/appmanager.h>

namespace game {

using namespace std;
using namespace flappy;

void MyGameManager::init() {
    Catch::Session session;
    session.configData().showDurations = Catch::ShowDurations::Always;
    auto args = MGR<AppManager>()->args();
    const char* argv[args.size()];
    for (unsigned i = 0; i < args.size(); i++)
        argv[i] = args[i].data();
    if (session.applyCommandLine(args.size(), argv) == 0)
        session.run();
    else
        throw runtime_error("Catch command line error.");
}

} // flappy
