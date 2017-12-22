#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>

#include "TestManager.h"
#include "TestComponent.h"
#include "DependOnComponentTestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Adding of Component to Entity") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    auto component = make_shared<TestComponent>(&mock.get());

    auto entity = make_shared<Entity>();

    REQUIRE(component->isInitialized() == false);

    entity->addComponent(component);
    // init on component adding
    Verify(Method(mock,init)).Exactly(1);

    REQUIRE(component->isInitialized() == true);

    entity->removeComponent(component);
    // deinit on remove from entity
    Verify(Method(mock,deinit)).Exactly(1);

    REQUIRE(component->isInitialized() == false);
}

TEST_CASE( "Init/deinit of component with another component as dependency") {
    Mock<DependOnComponentTestComponent::IMock> mock;
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    auto component = make_shared<DependOnComponentTestComponent>(&mock.get());

    auto entity = make_shared<Entity>();
    entity->addComponent(component);

    Verify(Method(mock,init)).Exactly(0);

    auto testComponent = entity->createComponent<TestComponent>();

    Verify(Method(mock,init)).Exactly(1);

    entity->removeComponent(testComponent);

    Verify(Method(mock,deinit)).Exactly(1);

    testComponent = entity->createComponent<TestComponent>();

    Verify(Method(mock,init)).Exactly(2);
}

TEST_CASE( "Component::setActive()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    auto component = make_shared<TestComponent>(&mock.get());

    auto entity = make_shared<Entity>();
    entity->addComponent(component);

    Verify(Method(mock,init)).Exactly(1);

    component->setActive(false);

    Verify(Method(mock,deinit)).Exactly(1);

    component->setActive(true);

    Verify(Method(mock,init)).Exactly(2);
}

TEST_CASE( "Component::entity()" ) {
    auto component = make_shared<TestComponent>();
    auto entity = make_shared<Entity>();

    REQUIRE(component->entityPtr() == nullptr);

    entity->addComponent(component);

    REQUIRE(component->entityPtr() == entity.get());

    entity->removeComponent(component);

    REQUIRE(component->entityPtr() == nullptr);

}

TEST_CASE( "Component::events()" ) {
    auto component = make_shared<TestComponent>();
    REQUIRE(component->events() != nullptr);
}

TEST_CASE("Passing events to children components") {

    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    {
        auto rootEntity = make_shared<Entity>();
        auto rootComponent = rootEntity->createComponent<TestComponent>();

        auto childEntity = rootEntity->createEntity();
        auto childComponent = make_shared<TestComponent>(&mock.get());
        childEntity->addComponent(childComponent);

        rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));

        Verify(Method(mock,update)).Exactly(1);
    }
    mock.get();

}
