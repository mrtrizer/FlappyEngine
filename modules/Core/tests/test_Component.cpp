#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <SceneManager.h>
#include <TransformComponent.h>
#include <SceneManager.h>

#include "TestManager.h"
#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Component::update()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));

    auto entity = make_shared<Entity>();
    auto testComponent = make_shared<TestComponent>(&mock.get());

    testComponent->update(1);

    Verify(Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "Component::init()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,init));

    shared_ptr<Component> component = make_shared<TestComponent>(&mock.get());

    component->init();

    Verify(Method(mock,init)).Exactly(1);
}

TEST_CASE( "Component::entity()" ) {
    auto entity = std::make_shared<Entity>();
    entity->component<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>()->entity().lock() == entity);
}

TEST_CASE( "Component::manager()" ) {

}
