#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/resmanager.h>

using namespace flappy;
using namespace std;

namespace {

class TestRes: public Res<TestRes> {
public:
    TestRes(std::string testStr): testStr(testStr) {}
    std::string testStr;
};

class TestResFactory: public IResFactory {
};

class LoadTestResFactory: public IResFactory {
     std::shared_ptr<IRes> load(const string&) override {
         return std::static_pointer_cast<IRes>(std::make_shared<TestRes>("loaded res"));
     }
};

}

// Tests for ResHandler
TEST_CASE( "ResHandler dependencies") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();

    // create resource 1
    resManager->set("test_res1", TestRes("test"));
    auto resRef1 = resManager->get<TestRes>("test_res1"); // resRef need to keep res alive
    REQUIRE(resRef1->updated() == true);
    REQUIRE(resRef1->loading() == false); // if was explicitly set, it will not be reloaded
    REQUIRE(resRef1->path() == "test_res1");
    int classId = ClassId<IRes, TestRes>::id();
    REQUIRE(resRef1->id() == classId);
    REQUIRE(resRef1->ready() == false);

    // update
    managerList->update();
    REQUIRE(resRef1->updated() == true);
    REQUIRE(resRef1->ready() == true); // should be ready before adding dependency

    // create and add dependency
    auto resRef2 = resManager->set("test_res2", TestRes("test_dependency"));
    resRef1->addDependency(resRef2);
    resRef2.reset(); // should not be removed after updated coz set as dependency for test_res1
    REQUIRE(resRef1->ready() == false);

    // update right after adding new dependency
    managerList->update();
    bool updated1 = resRef1->updated() == true;
    bool ready1 = resRef1->ready() == true;

    // update
    managerList->update();
    bool updated2 = resRef1->updated() == true;
    bool ready2 = resRef1->ready() == true;

    // all dependencies should be loaded after two updates
    // but update flag should be set only onece
    REQUIRE(((updated1 == true && ready1 == true)
            ^ (updated2 == true && ready2 == true)));

    // update
    managerList->update();
    REQUIRE(resRef1->ready() == true);
    { // set new resource to one of dependency
        auto resRef = resManager->set("test_res2", TestRes("test_dependency1"));
        REQUIRE(resRef->updated() == true);
    }

    // update
    managerList->update();
    REQUIRE(resRef1->updated() == true);
}
