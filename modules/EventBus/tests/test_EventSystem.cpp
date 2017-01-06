#include "catch.h"

#include <memory>

#include <EventBus.h>
#include <Tools.h>

using namespace flappy;

TEST_CASE( "EventSystem::subscribe() EventSystem::post()") {
    bool checkFlag = false;

    struct TestEvent {};
    EventBus eventSystem;
    { // Subscription life time
        auto subscription = eventSystem.subscribe([&checkFlag](TestEvent) {
            checkFlag = true;
        });
        eventSystem.post(TestEvent());
    }

    REQUIRE(checkFlag == true);
}

