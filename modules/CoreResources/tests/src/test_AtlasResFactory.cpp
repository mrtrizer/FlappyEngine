#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <AtlasRes.h>
#include <AtlasResFactory.h>
#include <ResManager.h>
#include <ResRepositoryManager.h>
#include <StdFileLoadManager.h>
#include <StdFileMonitorManager.h>
#include <JsonRes.h>
#include <TextRes.h>
#include <DefaultResFactory.h>
#include <FileResFactory.h>
#include <Tools.h>

using namespace flappy;
using namespace std;
using namespace Tools;

TEST_CASE( "AtlasResFactory loading from file") {
    auto entity = std::make_shared<Entity>();
    entity->createComponent<StdFileLoadManager>();
    entity->createComponent<StdFileMonitorManager>();
    entity->createComponent<FileResFactory>();
    entity->createComponent<ResManager<TextRes>>();
    entity->createComponent<DefaultResFactory<JsonRes, JsonRes, TextRes>>();
    entity->createComponent<ResManager<JsonRes>>();
    entity->createComponent<ResRepositoryManager>("./resources/");
    entity->createComponent<AtlasResFactory>();
    auto atlasResManager = entity->createComponent<ResManager<AtlasRes>>();
    auto atlasTest = atlasResManager->getResSync("test_atlas");
    entity->events()->post(ComponentBase::UpdateEvent(1.0f));
    entity->events()->post(ComponentBase::UpdateEvent(1.0f));
    entity->events()->post(ComponentBase::UpdateEvent(1.0f));
    atlasTest = atlasTest->lastRes();
    const auto expectedRect = Tools::Rect(0.1f, 0.2f, 0.3f, 0.4f);
    REQUIRE(atlasTest->spriteInfo("rect1").rectInAtlas == expectedRect);
    const auto expectedSize = glm::vec2(100, 200);
    REQUIRE(atlasTest->spriteInfo("rect1").size == expectedSize);
}
