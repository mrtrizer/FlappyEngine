#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/entity.h>
#include <core/transform.h>
#include <core/managerlist.h>

#include "testcomponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Entity::add()" ) {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>(managerList);
    REQUIRE(entity->get<TestComponent>() == nullptr);
    entity->add<TestComponentEmpty>();
    REQUIRE(entity->get<TestComponent>() == nullptr);
    auto component = entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>() == component);
    entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().size() == 2);
}

TEST_CASE( "Entity::get()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>(managerList);
    REQUIRE(entity->get<TestComponent>() == nullptr);
    entity->add<TestComponentEmpty>();
    REQUIRE(entity->get<TestComponent>() == nullptr);
    auto component = entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>() == component);
    entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>() == component); // should return first component
}

TEST_CASE( "Entity::getAll()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>(managerList);
    auto component1 = entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().front() == component1);
    auto component2 = entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().back() == component2);
    auto component3 = entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().back() == component3);
    REQUIRE(entity->getAll<TestComponent>().size() == 3);
}

TEST_CASE( "Entity::update()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>(managerList);
    entity->add<TestComponent>();

    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    entity->add<TestComponent>(&mock.get());
    entity->update(1);

    Verify(Method(mock,init), Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "Entity::transform()") {
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>(managerList);
    auto transform = entity->add<Transform>();
    entity->transform() == transform;
}
