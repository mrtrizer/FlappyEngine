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
#include <Entity.hpp>
#include <StdFileMonitorManager.h>
#include <StdFileLoadManager.h>
#include <ResRepositoryManager.h>
#include <ResManager.h>
#include <GLTextureRes.h>
#include <GLTextureResFactory.h>
#include <Heap.hpp>
#include <Hierarchy.hpp>

using namespace flappy;
using namespace std;

TEST_CASE( "SpriteResManager::getRes") {
    auto hierarchy = Heap::create<Hierarchy>(Heap::memoryManager());
    hierarchy->initManager<UpdateManager>();
    hierarchy->initManager<IFileMonitorManager, StdFileMonitorManager>();
    hierarchy->initManager<IFileLoadManager, StdFileLoadManager>();
    auto resRepositoryManager = hierarchy->initManager<ResRepositoryManager>();
    resRepositoryManager->setRepositoryPath("./resources");
    
    hierarchy->initManager<ResFactory<AtlasRes>, DefaultResFactory<AtlasRes, AtlasRes>> ();
    hierarchy->initManager<ResManager<AtlasRes>> ();
    hierarchy->initManager<ResFactory<IRgbaBitmapRes>, Sdl2RgbaBitmapResFactory> ();
    hierarchy->initManager<ResManager<IRgbaBitmapRes>> ();
    hierarchy->initManager<ResFactory<TextureRes>, GLTextureResFactory>();
    hierarchy->initManager<ResManager<TextureRes>> ();
    hierarchy->initManager<ResFactory<SpriteRes>, SpriteResFactory> ();
    hierarchy->initManager<ResManager<SpriteRes>> ();

    // We can't get this resource without Sdl2Manager initialization
//    rootEntity->manager<ResManager<SpriteRes>>()->getRes("test_img");
//    rootEntity->events()->post(ComponentBase::UpdateEvent(1));
}

TEST_CASE( "SpriteRes::rect()") {
    REQUIRE(true);
}

TEST_CASE( "SpriteRes::texture()") {
    REQUIRE(true);
}

