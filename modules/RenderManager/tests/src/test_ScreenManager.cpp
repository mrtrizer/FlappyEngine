#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ScreenManager.h>

using namespace flappy;
using namespace std;

TEST_CASE( "ScreenManager::resize() screenSize()") {
    ScreenManager screenManager;
    bool resized = false;
    auto subscription = screenManager.eventBus().subscribe([&resized](ScreenManager::ScreenResizedEvent) {
        resized = true;
    });
    const glm::uvec2 screenSize {100, 200};
    screenManager.resize(screenSize);
    REQUIRE(screenManager.screenSize() == screenSize);
    REQUIRE(resized == true);
}
