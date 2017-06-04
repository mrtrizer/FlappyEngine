#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <AppManager.h>
#include <Entity.h>

using namespace flappy;
using namespace std;


TEST_CASE( "AppManager::args()") {
    auto managerList1 = make_shared<Entity>();
    managerList1->createManager<AppManager>();
    REQUIRE(string(managerList1->findManager<AppManager>()->args()[0]) == "flappy");
    const char* args[] = {"test", "test_arg"};
    auto managerList2 = make_shared<Entity>();
    managerList2->createManager<AppManager>(2, args);
    REQUIRE(string(managerList2->findManager<AppManager>()->args()[1]) == "test_arg");
}
