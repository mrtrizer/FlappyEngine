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

    auto resRepositoryManager = rootEntity->createComponent<ResRepositoryManager>("./resources");

    rootEntity->createComponent<ResManager<AtlasRes>> ();
    rootEntity->createComponent<ResManager<IRgbaBitmapRes>> (Sdl2RgbaBitmapResFactory());
    rootEntity->createComponent<ResManager<TextureRes>> (DefaultResFactory<GLTextureRes, IRgbaBitmapRes>());
    rootEntity->createComponent<ResManager<QuadRes>> (QuadResFactory());

    rootEntity->manager<ResManager<QuadRes>>()->getRes("test_img");
    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
}

TEST_CASE( "Quad::rect()") {
    REQUIRE(true);
}

TEST_CASE( "Quad::texture()") {
    REQUIRE(true);
}

