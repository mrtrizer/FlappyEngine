#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <SpriteRes.h>
#include <AtlasRes.h>
#include <TextureRes.h>
#include <SpriteResFactory.h>
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

TEST_CASE( "SpriteResManager::getRes") {
    auto rootEntity = std::make_shared<Entity>();
    rootEntity->createComponent<StdFileMonitorManager>();
    rootEntity->createComponent<StdFileLoadManager>();

    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");

    rootEntity->createComponent<DefaultResFactory<AtlasRes, AtlasRes>> ();
    rootEntity->createComponent<ResManager<AtlasRes>> ();
    rootEntity->createComponent<Sdl2RgbaBitmapResFactory> ();
    rootEntity->createComponent<ResManager<IRgbaBitmapRes>> ();
    rootEntity->createComponent<DefaultResFactory<TextureRes, GLTextureRes, IRgbaBitmapRes>>();
    rootEntity->createComponent<ResManager<TextureRes>> ();
    rootEntity->createComponent<SpriteResFactory> ();
    rootEntity->createComponent<ResManager<SpriteRes>> ();

    rootEntity->manager<ResManager<SpriteRes>>()->getRes("test_img");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
}

TEST_CASE( "SpriteRes::rect()") {
    REQUIRE(true);
}

TEST_CASE( "SpriteRes::texture()") {
    REQUIRE(true);
}

