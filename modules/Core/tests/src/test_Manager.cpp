#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Manager.h>
#include <Entity.h>

#include "TestManager.h"
#include "DependantTestComponent.h"

using namespace flappy;
using namespace std;
using namespace fakeit;

TEST_CASE("Add/Remove manager events") {
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto componentMiddle = entityMiddle->createComponent<Component>();

    bool onAddedCalled = false;

    componentMiddle->events()->subscribeIn([&onAddedCalled](Component::OnManagerAdded e) {
        if (auto manager = e.castTo<TestManager>())
            onAddedCalled = true;
    });

    bool onRemovedCalled = false;

    componentMiddle->events()->subscribeIn([&onRemovedCalled](Component::OnManagerRemoved e) {
        if (auto manager = e.castTo<TestManager>())
            onRemovedCalled = true;
    });

    auto testManager = entityRoot->createComponent<TestManager>();
    REQUIRE(onAddedCalled == true);
    REQUIRE(onRemovedCalled == false);

    entityRoot->removeComponent(testManager);
    REQUIRE(onRemovedCalled == true);
}

TEST_CASE("Add/Remove component events") {
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto testManager = entityRoot->createComponent<TestManager>();

    auto componentMiddle = std::make_shared<Component>();

    bool onAddedCalled = false;

    componentMiddle->events()->subscribeIn([&onAddedCalled](Component::OnManagerAdded e) {
        if (auto manager = e.castTo<TestManager>())
            onAddedCalled = true;
    });

    bool onRemovedCalled = false;

    componentMiddle->events()->subscribeIn([&onRemovedCalled](Component::OnManagerRemoved e) {
        if (auto manager = e.castTo<TestManager>())
            onRemovedCalled = true;
    });

    entityMiddle->addComponent(componentMiddle);
    REQUIRE(onAddedCalled == true);
    REQUIRE(onRemovedCalled == false);

    entityMiddle->removeComponent(componentMiddle);
    REQUIRE(onRemovedCalled == true);
}

TEST_CASE( "Init/deinit of component with dependencies") {
    Mock<DependantTestComponent::IMock> mock;
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    auto component = make_shared<DependantTestComponent>(&mock.get());

    auto entity = make_shared<Entity>();
    entity->addComponent(component);

    Verify(Method(mock,init)).Exactly(0);

    auto testManager = entity->createComponent<TestManager>();

    Verify(Method(mock,init)).Exactly(1);

    entity->removeComponent(testManager);

    Verify(Method(mock,deinit)).Exactly(1);

    testManager = entity->createComponent<TestManager>();

    Verify(Method(mock,init)).Exactly(2);
}
