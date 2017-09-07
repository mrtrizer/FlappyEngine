#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ResManager.h>
#include <IResFactory.h>
#include <Res.h>
#include <Tools.h>
#include <Entity.h>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <ResRepositoryManager.h>
#include <FileResFactory.h>
#include <TextRes.h>

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

}

TEST_CASE( "ResRepositoryManager::findResInfo") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    auto resInfo = resRepositoryManager->findResInfo("text_res");
    REQUIRE(resInfo.type == "file");
}

TEST_CASE( "ResManager::getRes()") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<FileResFactory<TextRes>>();
    auto resManager = rootEntity->createComponent<ResManager<TextRes>>();
    auto textRes = resManager->getResSync("text_res");
    REQUIRE(textRes->text() == "Abuksigun\n");
}
