#include <catch.h>
#include <fakeit.h>

#include <memory>

#include <Utility.hpp>

using namespace flappy;
using namespace std;

TEST_CASE("sstr") {
    REQUIRE(sstr("1", '2', 3) == "123");
}
