#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.hpp>
#include <AtlasRes.h>
#include <AtlasResFactory.h>
#include <ResManager.h>
#include <ResRepositoryManager.h>
#include <StdFileLoadManager.h>
#include <StdFileMonitorManager.h>
#include <JsonRes.h>
#include <TextRes.h>
#include <DefaultResFactory.h>
#include <TextResFactory.h>
#include <Utility.hpp>
#include <Heap.hpp>
#include <Hierarchy.hpp>
#include <MathUtils.h>

using namespace flappy;
using namespace std;

TEST_CASE( "AtlasResFactory loading from file") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    hierarchy->initManager<ResFactory<TextRes>, TextResFactory>();
    auto textResManager = hierarchy->initManager<ResManager<TextRes>>();
    hierarchy->initManager<ResFactory<JsonRes>, DefaultResFactory<JsonRes, JsonRes, TextRes>>();
    auto jsonResManager = hierarchy->initManager<ResManager<JsonRes>>();
    auto updateManager = hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<ResFactory<AtlasRes>, AtlasResFactory>();
    auto atlasResManager = hierarchy->initManager<ResManager<AtlasRes>>();
    auto atlasTest = atlasResManager->getRes("test_atlas", ExecType::SYNC);
    atlasTest = atlasTest->lastRes();
    const auto expectedRect = MathUtils::Rect(0.1f, 0.2f, 0.3f, 0.4f);
    REQUIRE(atlasTest->spriteInfo("rect1").rectInAtlas == expectedRect);
    const auto expectedSize = glm::vec2(100, 200);
    REQUIRE(atlasTest->spriteInfo("rect1").size == expectedSize);
}
