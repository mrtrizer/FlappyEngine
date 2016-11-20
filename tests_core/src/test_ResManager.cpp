#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <managers/ResManager/ResManager.h>
#include <managers/ResManager/ResFactory.h>
#include <managers/ResManager/Res.h>
#include <core/tools.h>

using namespace flappy;
using namespace std;

namespace {

class TestRes: public Res {
public:
    TestRes(std::string testStr): testStr(testStr) {}
    std::list<std::shared_ptr<Res>> dependencyList() const override {
        return {};
    }
    std::string testStr;
};

class TestResFactory: public ResFactory {
    std::shared_ptr<Res> load(const std::string& name, std::shared_ptr<ResManager>) {
        return std::make_shared<TestRes>(name + " loaded");
    }

    std::shared_ptr<Res> create(const std::string& name, std::shared_ptr<ResManager>) {
        return std::make_shared<TestRes>(name + " default");
    }

    bool changed(const std::string&) {
        return false;
    }
};

}

TEST_CASE( "ResManager::setRes()") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    resManager->setRes<::TestRes>("test", make_shared<::TestRes>("set res"));
}

TEST_CASE( "ResManager::getRes()") {
    auto managerList = make_shared<ManagerList>();
    auto resManager = managerList->create<ResManager>();
    resManager->bindResFactory<::TestRes>(make_shared<::TestResFactory>());
    auto res = resManager->getRes<TestRes>("test");
}
