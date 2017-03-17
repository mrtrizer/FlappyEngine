#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <TransformComponent.h>

#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

// Base

TEST_CASE( "root() parent()" )
{
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto entityLast = entityMiddle->createEntity();

    REQUIRE(entityLast->root() == entityRoot);
    REQUIRE(entityLast->parent() == entityMiddle);
}

TEST_CASE( "events() Events sending" )
{
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto entityLast = entityMiddle->createEntity();

    struct TestEvent: public IEvent {
        int value;
    };

    int resultValue = 0;

    entityLast->events()->subscribeIn([&resultValue](const TestEvent& e) {
        resultValue = e.value;
    });

    TestEvent event;
    event.value = 10;
    entityRoot->events()->post(event);

    // Check event passed
    REQUIRE(resultValue == 10);
}


// Component managment

TEST_CASE("addComponent(std::shared_ptr<ComponentT> )") {
    auto testEntity = std::make_shared<Entity>();
    auto testComponent = std::make_shared<Component>();

    testEntity->addComponent(testComponent);
    REQUIRE_THROWS(testEntity->addComponent(testComponent));
}

TEST_CASE("createComponent(Args ... args)") {

}

TEST_CASE("removeComponent(std::shared_ptr<ComponentT> )") {

}

TEST_CASE("findComponent(predicate, depth) findComponent(depth)") {

}

TEST_CASE("findComponents(predicate, depth) findComponents(depth)") {

}

TEST_CASE("component<ComponentT>()") {

}


// Manager managment

TEST_CASE("addManager(std::shared_ptr<ManagerT>) manager<ManagerT>()") {

}

TEST_CASE("createManager(Args ... args)") {

}

TEST_CASE("removeManager(std::shared_ptr<ManagerT> manager)") {

}


// Entity managment

TEST_CASE("addEntity(std::shared_ptr<Entity>)") {

}

TEST_CASE("removeEntity(std::shared_ptr<Entity>)") {

}

TEST_CASE("findEntity(predicate, depth)") {

}

TEST_CASE("findEntities(depth) findEntities(predicate, depth)") {

}
