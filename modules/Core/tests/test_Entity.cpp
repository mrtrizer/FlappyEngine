#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <TransformComponent.h>
#include <ManagerList.h>

#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Entity::component()" ) {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    entity->createComponent<TestComponentEmpty>();
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    auto component = entity->createComponent<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>() == component);
    entity->createComponent<TestComponent>();
    REQUIRE(entity->components<TestComponent>().size() == 2);
}

TEST_CASE( "Entity::findComponent()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    entity->createComponent<TestComponentEmpty>();
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    auto component = entity->createComponent<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>() == component);
    entity->createComponent<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>() == component); // should return first component
}

TEST_CASE( "Entity::getAll()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    auto component1 = entity->createComponent<TestComponent>();
    REQUIRE(entity->components<TestComponent>().front() == component1);
    auto component2 = entity->createComponent<TestComponent>();
    REQUIRE(entity->components<TestComponent>().back() == component2);
    auto component3 = entity->createComponent<TestComponent>();
    REQUIRE(entity->components<TestComponent>().back() == component3);
    REQUIRE(entity->components<TestComponent>().size() == 3);
}

TEST_CASE( "Entity::update()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    entity->createComponent<TestComponent>();

    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    entity->createComponent<TestComponent>(&mock.get());
    entity->update(1);

    Verify(Method(mock,init), Method(mock,update).Using(1)).Exactly(1);
}
