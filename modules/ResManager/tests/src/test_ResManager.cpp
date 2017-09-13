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
    TestRes(std::string testStr)
        :testStr(testStr)
    {}
    std::string testStr;
};

}

TEST_CASE( "ResRepositoryManager::findFileInfo") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    auto resInfo = resRepositoryManager->findFileInfo("dir/text_res.txt");
    REQUIRE(resInfo.type == "file");
}

TEST_CASE( "ResRepositoryManager::findResMeta") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    auto resMeta = resRepositoryManager->findResMeta("dir/text_res");
    REQUIRE(resMeta.data["input"] == "dir/text_res.txt");
    REQUIRE(resMeta.data["type"] == "file");
}

TEST_CASE( "ResManager::getRes()") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    rootEntity->createComponent<FileResFactory<TextRes>>();
    auto resManager = rootEntity->createComponent<ResManager<TextRes>>();
    auto textRes = resManager->getResSync("dir/text_res");
    REQUIRE(textRes->text() == "Abuksigun\n");
}
