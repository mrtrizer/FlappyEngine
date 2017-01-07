#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <AtlasRes.h>
#include <Tools.h>

using namespace flappy;
using namespace std;
using namespace Tools;

TEST_CASE( "AtlasRes::addRect() AtlasRes::rect() default") {
    AtlasRes atlas;
    auto defaultRect = Rect{0.0f,0.0f,1.0f,1.0f};
    auto defaultSize = glm::vec2{0.0f, 0.0f};
    auto spriteInfo = atlas.spriteInfo("sprite1");
    REQUIRE(spriteInfo.rectInAtlas == defaultRect);
    REQUIRE(spriteInfo.size == defaultSize);
}

TEST_CASE( "AtlasRes::addRect() AtlasRes::rect()") {
    AtlasRes atlas;
    { // first adding
        auto spriteInfo1 = AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,0.5f}, {1.0f, 1.0f});
        atlas.addSpriteInfo("sprite1",spriteInfo1);
        auto spriteInfo = atlas.spriteInfo("sprite1");
        REQUIRE(spriteInfo.rectInAtlas == spriteInfo1.rectInAtlas);
        REQUIRE(spriteInfo.size == spriteInfo1.size);
    }
    { // multiple elements
        auto spriteInfo2 = AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,0.5f}, {1.0f, 1.0f});
        atlas.addSpriteInfo("sprite2",spriteInfo2);
        auto spriteInfo = atlas.spriteInfo("sprite2");
        REQUIRE(spriteInfo.rectInAtlas == spriteInfo2.rectInAtlas);
        REQUIRE(spriteInfo.size == spriteInfo2.size);
    }
}
