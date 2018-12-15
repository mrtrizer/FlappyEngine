#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <MathUtils.h>

using namespace flappy;

TEST_CASE("Math::isPowOfTwo()") {
    REQUIRE(MathUtils::isPowOfTwo(8) == true);
    REQUIRE(MathUtils::isPowOfTwo(13) == false);
}
