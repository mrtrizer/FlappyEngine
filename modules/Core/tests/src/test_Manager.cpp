#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Manager.h>
#include <Entity.h>

#include "TestManager.h"

using namespace flappy;
using namespace std;
using namespace fakeit;

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


class DependantTestComponent: public flappy::Component {
public:
    class IMock
    {
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
    };

    DependantTestComponent(IMock* mockComponent = nullptr):
        Component({ClassId<Component, TestManager>().id()}),
        m_mockComponent (mockComponent){

    }

    void init() override final {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }
private:
    IMock* m_mockComponent;
};

TEST_CASE( "Initialization of component with dependencies") {
    Mock<DependantTestComponent::IMock> mock;
    Fake(Method(mock,init));

    auto component = make_shared<DependantTestComponent>(&mock.get());

    auto entity = make_shared<Entity>();
    entity->addComponent(component);

    Verify(Method(mock,init)).Exactly(0);

    auto testManager = entity->createManager<TestManager>();

    Verify(Method(mock,init)).Exactly(1);
}
