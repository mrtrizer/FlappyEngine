#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/appmanager.h>

using namespace flappy;
using namespace std;


TEST_CASE( "AppManager::args()") {
    auto managerList1 = make_shared<ManagerList>();
    managerList1->create<AppManager>();
    REQUIRE(string(managerList1->MGR<AppManager>()->args()[0]) == "flappy");
    const char* args[] = {"test", "test_arg"};
    auto managerList2 = make_shared<ManagerList>();
    managerList2->create<AppManager>(2, args);
    REQUIRE(string(managerList2->MGR<AppManager>()->args()[1]) == "test_arg");
}
