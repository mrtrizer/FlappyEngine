#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ResManager.h>
#include <ResFactory.h>
#include <Res.h>
#include <Tools.h>
#include <Entity.h>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <ResRepositoryManager.h>

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

TEST_CASE( "ResRepositoryManager::findResInfo") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>("./resources");
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("res_list.json");
    auto resInfo = resRepositoryManager->findResInfo("text_res.txt");
    REQUIRE(resInfo.type == "file");
}

TEST_CASE( "ResManager::setRes()") {

}

TEST_CASE( "ResManager::getRes()") {
//    auto managerList = make_shared<ManagerList>();
//    auto resManager = managerList->create<ResManager>();
//    resManager->bindResFactory<::TestRes>(make_shared<::TestResFactory>());
//    auto res = resManager->getRes<TestRes>("test");
}
