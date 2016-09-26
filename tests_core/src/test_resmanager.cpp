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

TEST_CASE( "ResManager::set()") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    resManager->set("test_res", TestRes("test"));
    auto resRef = resManager->get<TestRes>("test_res"); // resRef need to keep res alive
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    managerList->update();
    REQUIRE(resManager->get<TestRes>("test_res")->resource() != nullptr);
    REQUIRE(resManager->get<TestRes>("test_res")->resource()->testStr == "test");
    resRef.reset(); // reset resRef to destroy res in next update
    managerList->update();
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
}

TEST_CASE( "ResManager::get") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    resManager->bind<::TestRes>(make_shared<::LoadTestResFactory>());
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    auto resRef = resManager->get<TestRes>("test_res");
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    managerList->update();
    REQUIRE(resManager->get<TestRes>("test_res")->resource() != nullptr);
    REQUIRE(resManager->get<TestRes>("test_res")->resource()->testStr == "loaded res");
    managerList->update(); // nothing changes after second update
    REQUIRE(resManager->get<TestRes>("test_res")->resource() != nullptr);
    REQUIRE(resManager->get<TestRes>("test_res")->resource()->testStr == "loaded res");
}

// Tests for ResHandler

TEST_CASE( "ResHandler dependencies") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    resManager->set("test_res1", TestRes("test"));

    auto resRef1 = resManager->get<TestRes>("test_res1"); // resRef need to keep res alive
    REQUIRE(resRef1->updated() == false); // if was explicitly set, it will not be reloaded
    REQUIRE(resRef1->loading() == false);
    REQUIRE(resRef1->path() == "test_res1");
    int classId = ClassId<IRes, TestRes>::id();
    REQUIRE(resRef1->id() == classId);
    auto resRef2 = resManager->set("test_res2", TestRes("test_dependency"));
    resRef1->addDependency(resRef2);
    resRef2.reset(); // should not be removed after updated coz set as dependency for test_res1
    REQUIRE(resManager->get<TestRes>("test_res1")->resource() == nullptr);
    REQUIRE(resRef1->ready() == false);


    managerList->update();
}
