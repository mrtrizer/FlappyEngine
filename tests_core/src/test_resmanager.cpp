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
     std::shared_ptr<IRes> load(const string&) override {
         return std::static_pointer_cast<IRes>(std::make_shared<TestRes>("loaded res"));
     }
};

}

TEST_CASE( "ResManager::set()") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    resManager->set("test_res", TestRes("test"));
    resManager->get<TestRes>("test_res");
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    managerList->update();
    REQUIRE(resManager->get<TestRes>("test_res")->resource() != nullptr);
    REQUIRE(resManager->get<TestRes>("test_res")->resource()->testStr == "test");
}

TEST_CASE( "ResManager::get") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    resManager->bind<::TestRes>(make_shared<::TestResFactory>());
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    resManager->get<TestRes>("test_res");
    REQUIRE(resManager->get<TestRes>("test_res")->resource() == nullptr);
    managerList->update();
    REQUIRE(resManager->get<TestRes>("test_res")->resource() != nullptr);
    REQUIRE(resManager->get<TestRes>("test_res")->resource()->testStr == "loaded res");
}
