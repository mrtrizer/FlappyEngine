#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ResManager.h>
#include <ResFactory.h>
#include <Res.h>
#include <DebugServices.h>
#include <Heap.hpp>
#include <Entity.hpp>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <ResRepositoryManager.h>
#include <TextResFactory.h>
#include <TextRes.h>

using namespace flappy;
using namespace std;

namespace {

class TestRes: public Res<TestRes> {
public:
    TestRes(std::string testStr)
        :testStr(testStr)
    {}
    std::string testStr;
};

}

TEST_CASE( "ResRepositoryManager::findFileInfo") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    hierarchy->initManager<UpdateManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    auto resInfo = resRepositoryManager->findFileInfo("dir/text_res.txt");
    REQUIRE(resInfo.type == "file");
}

TEST_CASE( "ResRepositoryManager::findResMeta") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    hierarchy->initManager<UpdateManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    auto resMeta = resRepositoryManager->findResMeta("dir/text_res");
    REQUIRE(resMeta.data["input"] == "dir/text_res.txt");
    REQUIRE(resMeta.data["type"] == "file");
}

TEST_CASE( "ResManager::getRes()") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    hierarchy->initManager<UpdateManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    hierarchy->initManager<ResFactory<TextRes>, TextResFactory>();
    auto resManager = hierarchy->initManager<ResManager<TextRes>>();
    auto textRes = resManager->getRes("dir/text_res", ExecType::SYNC);
    REQUIRE(textRes->text() == "Abuksigun\n");
}
