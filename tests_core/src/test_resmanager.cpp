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
