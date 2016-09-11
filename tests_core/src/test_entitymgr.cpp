#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/entitymanager.h>

#include "testcomponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "EntityManager::update()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    EntityManager entityManager;
    auto entity1 = entityManager.create();
    entity1->create<TestComponent>(&mock.get());
    entityManager.update(1);

    Verify(Method(mock,init), Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "EntityManager::remove()") {
    EntityManager entityManager;
    auto entity1 = entityManager.create();
    REQUIRE(entityManager.entities().size() == 1);
    entityManager.remove(entity1);
    entityManager.update(1);
    REQUIRE(entityManager.entities().size() == 0);
}

TEST_CASE( "EntityManager::reset()") {
    EntityManager entityManager;
    auto entity1 = entityManager.create();
    REQUIRE(entityManager.entities().size() == 1);
    entityManager.reset();
    entityManager.update(1);
    REQUIRE(entityManager.entities().size() == 0);
}

TEST_CASE( "EntityManager::create()") {
    EntityManager entityManager;
    auto entity1 = entityManager.create();
    REQUIRE( entityManager.entities().size() == 1);
}

TEST_CASE( "EntityManager::create(std::function)") {
    EntityManager entityManager;
    auto entity1 = entityManager.create([](EP e){
        e->create<TestComponent>();
    });
    REQUIRE(entityManager.entities().size() == 1);
}

TEST_CASE( "EntityManager::find()") {
    EntityManager entityManager;
    REQUIRE(entityManager.find([](EP){return true;}) == nullptr);
    REQUIRE(entityManager.find([](EP){return false;}) == nullptr);
    auto entity1 = entityManager.create();
    auto entity2 = entityManager.create();
    auto entity3 = entityManager.create();
    REQUIRE(entityManager.find([entity2](EP e){return e == entity2;}) == entity2);
    REQUIRE(entityManager.find([](EP){return false;}) == nullptr);
    entityManager.remove(entity3);
    entityManager.update(1);
    REQUIRE(entityManager.find([entity3](EP e){return e == entity3;}) == nullptr);
}

TEST_CASE( "EntityManager::findAll()") {
    EntityManager entityManager;
    REQUIRE(entityManager.findAll([](EP){return true;}).size() == 0);
    REQUIRE(entityManager.findAll([](EP){return false;}).size() == 0);
    auto entity1 = entityManager.create();
    auto entity2 = entityManager.create();
    REQUIRE(entityManager.entities().size() == 2);
    REQUIRE(entityManager.findAll([](EP){return true;}).size() == 2);
    REQUIRE(entityManager.findAll([](EP){return false;}).size() == 0);
}

TEST_CASE( "EntityManager::entities()") {
    EntityManager entityManager;
    auto entity1 = entityManager.create();
    auto entity2 = entityManager.create();
    REQUIRE(entityManager.entities().size() == 2);
    REQUIRE(entityManager.entities().back() == entity2);
    REQUIRE(entityManager.entities().front() == entity1);
}

TEST_CASE( "EntityManager::each() [empty list]") {
    EntityManager entityManager;
    unsigned componentCount = 0;
    entityManager.each<TestComponent>([&componentCount](EP){
        componentCount++;
    });
    REQUIRE(componentCount == 0);
}

TEST_CASE( "EntityManager::each() [one component]") {
    EntityManager entityManager;
    unsigned componentCount = 0;

    auto entity1 = entityManager.create();
    auto entity2 = entityManager.create();
    auto entity3 = entityManager.create();
    auto entity4 = entityManager.create();
    entity1->create<TestComponent>();
    entity2->create<TestComponent>();
    entity3->create<TestComponentEmpty>();

    entityManager.each<TestComponent>([&componentCount](EP){
        componentCount++;
    });
    REQUIRE(componentCount == 2);
}

TEST_CASE( "EntityManager::each() [multiple components]") {
    EntityManager entityManager;
    unsigned componentCount = 0;
    auto entity1 = entityManager.create();
    auto entity2 = entityManager.create();
    auto entity3 = entityManager.create();
    auto entity4 = entityManager.create();
    entity1->create<TestComponent>();
    entity2->create<TestComponent>();
    entity2->create<TestComponentEmpty>();
    entity3->create<TestComponentEmpty>();

    entityManager.each<TestComponent, TestComponentEmpty>([&componentCount](EP){
        componentCount++;
    });
    REQUIRE(componentCount == 1);
}
