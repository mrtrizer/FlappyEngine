#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>

#include "TestManager.h"
#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Component::setParentEntity() Component::init() Component::deinit() Component::isInitialized()") {
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

TEST_CASE( "Component::entity()" ) {
    auto component = make_shared<TestComponent>();
    auto entity = make_shared<Entity>();

    REQUIRE(component->entity() == nullptr);

    entity->addComponent(component);

    REQUIRE(component->entity() == entity);

    entity->removeComponent(component);

    REQUIRE(component->entity() == nullptr);

}

TEST_CASE( "Component::events()" ) {
    auto component = make_shared<TestComponent>();
    REQUIRE(component->events() != nullptr);
}

TEST_CASE("Passing events to children components") {
    auto rootEntity = make_shared<Entity>();
    auto rootComponent = rootEntity->createComponent<TestComponent>();

    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    auto childEntity = rootEntity->createEntity();
    auto childComponent = make_shared<TestComponent>(&mock.get());
    childEntity->addComponent(childComponent);

    rootEntity->events()->post(ComponentBase::UpdateEvent(1.0f));

    Verify(Method(mock,update)).Exactly(1);
}
