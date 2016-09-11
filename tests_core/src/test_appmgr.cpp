#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/appmgr.h>

using namespace flappy;
using namespace std;


TEST_CASE( "AppMgr::args()") {
    auto managerList1 = make_shared<ManagerList>();
    managerList1->create<AppMgr>();
    REQUIRE(string(managerList1->MGR<AppMgr>()->args()[0]) == "flappy");
    const char* args[] = {"test", "test_arg"};
    auto managerList2 = make_shared<ManagerList>();
    managerList2->create<AppMgr>(2, args);
    REQUIRE(string(managerList2->MGR<AppMgr>()->args()[1]) == "test_arg");
}
