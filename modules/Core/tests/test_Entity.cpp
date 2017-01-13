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

TEST_CASE( "Entity::add()" ) {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    entity->create<TestComponentEmpty>();
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    auto component = entity->create<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>() == component);
    entity->create<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().size() == 2);
}

TEST_CASE( "Entity::get()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    entity->create<TestComponentEmpty>();
    REQUIRE(entity->findComponent<TestComponent>() == nullptr);
    auto component = entity->create<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>() == component);
    entity->create<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>() == component); // should return first component
}

TEST_CASE( "Entity::getAll()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    auto component1 = entity->create<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().front() == component1);
    auto component2 = entity->create<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().back() == component2);
    auto component3 = entity->create<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().back() == component3);
    REQUIRE(entity->getAll<TestComponent>().size() == 3);
}

TEST_CASE( "Entity::update()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    entity->create<TestComponent>();

    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    entity->create<TestComponent>(&mock.get());
    entity->update(1);

    Verify(Method(mock,init), Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "Entity::transform()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    auto transform = entity->create<TransformComponent>();
    entity->transform() == transform;
}
