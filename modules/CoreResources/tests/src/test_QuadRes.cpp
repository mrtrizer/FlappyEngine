#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <QuadRes.h>
#include <AtlasRes.h>
#include <TextureRes.h>
#include <QuadResFactory.h>
#include <IRgbaBitmapRes.h>
#include <Sdl2RgbaBitmapResFactory.h>
#include <DefaultResFactory.h>
#include <Entity.h>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <ResRepositoryManager.h>
#include <ResManager.h>
#include <GLTextureRes.h>

using namespace flappy;
using namespace std;

TEST_CASE( "Quad ResManager::getRes") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();
    auto atlasResManager = rootEntity->createComponent<ResManager<AtlasRes>>();
    auto rgbaResManager = rootEntity->createComponent<ResManager<IRgbaBitmapRes>>();
    rgbaResManager->bindResFactory(std::make_shared<Sdl2RgbaBitmapResFactory>());
    auto textureResManager = rootEntity->createComponent<ResManager<TextureRes>>();
    textureResManager->bindResFactory(std::make_shared<DefaultResFactory<GLTextureRes, IRgbaBitmapRes>>());
    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");
    auto resManager = rootEntity->createComponent<ResManager<QuadRes>>();
    resManager->bindResFactory(std::make_shared<QuadResFactory>());
    auto textRes = resManager->getRes("test_img");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
}

TEST_CASE( "Quad::rect()") {
    REQUIRE(true);
}

TEST_CASE( "Quad::texture()") {
    REQUIRE(true);
}

