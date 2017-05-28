#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>

#include "TestManager.h"
#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Component::setParentEntity() Component::init() Component::deinit()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,init));
    Fake(Method(mock,deinit));

    auto component = make_shared<TestComponent>(&mock.get());

    auto entity = make_shared<Entity>();
    entity->addComponent(component);
    entity->removeComponent(component);


    Verify(Method(mock,init)).Exactly(1);
}

TEST_CASE( "Component::entity()" ) {
    auto entity = std::make_shared<Entity>();
    entity->component<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>()->entity() == entity);
}

TEST_CASE( "Component::manager()" ) {

}
