#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Math.h>

using namespace flappy;

TEST_CASE("Math::isPowOfTwo()") {
    REQUIRE(Math::isPowOfTwo(8) == true);
    REQUIRE(Math::isPowOfTwo(13) == false);
}
