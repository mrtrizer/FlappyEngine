#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>

#include <OpenALManager.h>

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE("OpenAL") {
    std::make_shared<OpenALManager>();
}
