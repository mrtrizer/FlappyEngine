#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Manager.h>
#include <Entity.h>

#include "TestManager.h"

using namespace flappy;
using namespace std;

TEST_CASE("Add/Remove manager events") {
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto componentMiddle = entityMiddle->createComponent<Component>();

    bool onAddedCalled = false;

    componentMiddle->events()->subscribeIn([&onAddedCalled](AManager::OnManagerAdded e) {
        if (auto manager = e.castTo<TestManager>())
            onAddedCalled = true;
    });

    bool onRemovedCalled = false;

    componentMiddle->events()->subscribeIn([&onRemovedCalled](AManager::OnManagerRemoved e) {
        if (auto manager = e.castTo<TestManager>())
            onRemovedCalled = true;
    });

    auto testManager = entityRoot->createManager<TestManager>();
    REQUIRE(onAddedCalled == true);
    REQUIRE(onRemovedCalled == false);

    entityRoot->removeManager(testManager);
    REQUIRE(onRemovedCalled == true);
}

TEST_CASE("Add/Remove component events") {
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto testManager = entityRoot->createManager<TestManager>();

    auto componentMiddle = std::make_shared<Component>();

    bool onAddedCalled = false;

    componentMiddle->events()->subscribeIn([&onAddedCalled](AManager::OnManagerAdded e) {
        if (auto manager = e.castTo<TestManager>())
            onAddedCalled = true;
    });

    bool onRemovedCalled = false;

    componentMiddle->events()->subscribeIn([&onRemovedCalled](AManager::OnManagerRemoved e) {
        if (auto manager = e.castTo<TestManager>())
            onRemovedCalled = true;
    });

    entityMiddle->addComponent(componentMiddle);
    REQUIRE(onAddedCalled == true);
    REQUIRE(onRemovedCalled == false);

    entityMiddle->removeComponent(componentMiddle);
    REQUIRE(onRemovedCalled == true);
}
