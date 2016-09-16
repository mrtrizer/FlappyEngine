#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/tools.h>

using namespace flappy;
using namespace std;

TEST_CASE( "Tools::isPowOfTwo()") {
    REQUIRE(Tools::isPowOfTwo(8) == true);
    REQUIRE(Tools::isPowOfTwo(13) == false);
}
