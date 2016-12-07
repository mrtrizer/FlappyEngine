#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <AtlasRes.h>
#include <Tools.h>

using namespace flappy;
using namespace std;
using namespace Tools;

TEST_CASE( "AtlasRes::addRect() AtlasRes::rect()") {
    AtlasRes atlas;
    auto defaultRect = Rect{0,0,1,1};
    REQUIRE(atlas.rect("rect1").x1 == defaultRect.x1);
    REQUIRE(atlas.rect("rect1").x2 == defaultRect.x2);
    REQUIRE(atlas.rect("rect1").y1 == defaultRect.y1);
    REQUIRE(atlas.rect("rect1").y2 == defaultRect.y2);
    auto rect1 = Rect{0,0,1,0.5f};
    atlas.addRect("rect1",rect1);
    REQUIRE(atlas.rect("rect1").x1 == rect1.x1);
    REQUIRE(atlas.rect("rect1").x2 == rect1.x2);
    REQUIRE(atlas.rect("rect1").y1 == rect1.y1);
    REQUIRE(atlas.rect("rect1").y2 == rect1.y2);
    auto rect2 = Rect{0,0.5f,1,1};
    atlas.addRect("rect2",rect2);
    REQUIRE(atlas.rect("rect2").x1 == rect2.x1);
    REQUIRE(atlas.rect("rect2").x2 == rect2.x2);
    REQUIRE(atlas.rect("rect2").y1 == rect2.y1);
    REQUIRE(atlas.rect("rect2").y2 == rect2.y2);
}
