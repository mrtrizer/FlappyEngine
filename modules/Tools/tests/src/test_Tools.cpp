#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Tools.h>

using namespace flappy;
using namespace std;

TEST_CASE( "Tools::isPowOfTwo()") {
    REQUIRE(Tools::isPowOfTwo(8) == true);
    REQUIRE(Tools::isPowOfTwo(13) == false);
}

TEST_CASE( "Tools::format()") {
    auto str = Tools::format("Test %d test %s", 5, "123");
    REQUIRE(str == "Test 5 test 123");
}
