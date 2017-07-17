#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>

#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;


// Component managment

TEST_CASE("addComponent(std::shared_ptr<ComponentT> )") {
    auto testEntity = std::make_shared<Entity>();
    auto testComponent = std::make_shared<TestComponent>();

    testEntity->addComponent(testComponent);
    REQUIRE_THROWS(testEntity->addComponent(testComponent));
    REQUIRE(testEntity->findComponent<TestComponent>() == testComponent);
}

TEST_CASE("createComponent(Args ... args)") {
    auto testEntity = std::make_shared<Entity>();
    auto testComponent = testEntity->createComponent<TestComponent>();

    REQUIRE_THROWS(testEntity->addComponent(testComponent));
    REQUIRE(*testEntity->findComponents<TestComponent>(0).begin() == testComponent);
}

TEST_CASE("removeComponent(std::shared_ptr<ComponentT> )") {
    auto testEntity = std::make_shared<Entity>();
    auto testComponent = testEntity->createComponent<TestComponent>();

    REQUIRE_NOTHROW(testEntity->removeComponent(testComponent));
    REQUIRE(testEntity->findComponents<TestComponent>(0).size() == 0);
}

TEST_CASE("findComponent<ComponentT>()") {
    auto entityRoot = std::make_shared<Entity>();
    auto componentRoot = entityRoot->createComponent<TestComponent>();
    auto entityMiddle = entityRoot->createEntity();
    auto componentMiddle = entityMiddle->createComponent<TestComponent>();

    REQUIRE(entityRoot->findComponent<TestComponent>() == componentRoot);
    REQUIRE(entityRoot->findComponent<TestComponent>(1) == componentRoot);
    {
        auto component = entityRoot->findComponent<TestComponent>([componentMiddle](const TestComponent& component) {
            return component.shared_from_this() == componentMiddle;
        },1);
        REQUIRE(component == componentMiddle);
    }
    {
        auto component = entityRoot->findComponent<TestComponent>([componentRoot](const TestComponent& component) {
            return component.shared_from_this() == componentRoot;
        },1);
        REQUIRE(component == componentRoot);
    }
}

TEST_CASE("findComponents<ComponentT>()") {
    auto entityRoot = std::make_shared<Entity>();
    auto componentRoot = entityRoot->createComponent<TestComponent>();
    auto entityMiddle = entityRoot->createEntity();
    auto componentMiddle = entityMiddle->createComponent<TestComponent>();

    {
        auto components = entityRoot->findComponents<TestComponent>([componentMiddle](const TestComponent& component) {
            return component.shared_from_this() == componentMiddle;
        },1);
        REQUIRE(components.size() == 1);
        REQUIRE(*components.begin() == componentMiddle);
    }
    {
        auto components = entityRoot->findComponents<TestComponent>([componentRoot](const TestComponent& component) {
            return component.shared_from_this() == componentRoot;
        },1);
        REQUIRE(components.size() == 1);
        REQUIRE(*components.begin() == componentRoot);
    }
}

TEST_CASE("component<ComponentT>()") {
    auto testEntity = std::make_shared<Entity>();
    auto component = testEntity->component<TestComponent>();
    REQUIRE(component == testEntity->component<TestComponent>());
}

// Entity managment

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

TEST_CASE("createEntity(std::shared_ptr<Entity>)") {
    auto entityRoot = std::make_shared<Entity>();
    auto entity = entityRoot->createEntity();
    auto entities = entityRoot->findEntities(0);
    REQUIRE(*entities.begin() == entity);
}

TEST_CASE("addEntity(std::shared_ptr<Entity>)") {
    auto entityRoot = std::make_shared<Entity>();
    auto entity = std::make_shared<Entity>();
    entityRoot->addEntity(entity);
    // can't add twice
    REQUIRE_THROWS(entityRoot->addEntity(entity));
    auto entities = entityRoot->findEntities(0);
    REQUIRE(*entities.begin() == entity);
}

TEST_CASE("removeEntity(std::shared_ptr<Entity>)") {
    auto entityRoot = std::make_shared<Entity>();
    auto entity = entityRoot->createEntity();
    auto entities = entityRoot->findEntities(0);
    REQUIRE(*entities.begin() == entity);
    entityRoot->removeEntity(entity);
    REQUIRE_NOTHROW(entityRoot->removeEntity(entity));
    REQUIRE(entityRoot->findEntities(0).size() == 0);
}

TEST_CASE("findEntity(predicate, depth)") {
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto entityLast = entityMiddle->createEntity();
    {
        auto entity = entityRoot->findEntity([entityMiddle](const Entity& entity) {
            return entity.shared_from_this() == entityMiddle;
        });
        REQUIRE(entity == entityMiddle);
    }
    {
        auto entity = entityRoot->findEntity([entityRoot](const Entity& entity) {
            return entity.shared_from_this() == entityRoot;
        });
        REQUIRE(entity == nullptr);
    }
    {
        auto entity = entityRoot->findEntity([entityMiddle](const Entity& entity) {
            return entity.shared_from_this() == entityMiddle;
        }, 1);
        REQUIRE(entity == entityMiddle);
    }
    {
        auto entity = entityRoot->findEntity([entityLast](const Entity& entity) {
            return entity.shared_from_this() == entityLast;
        }, 1);
        REQUIRE(entity == entityLast);
    }
}

TEST_CASE("findEntities(depth) findEntities(predicate, depth)") {
    auto entityRoot = std::make_shared<Entity>();
    auto entityMiddle = entityRoot->createEntity();
    auto entityLast = entityMiddle->createEntity();
    {
        auto entities = entityRoot->findEntities([](const Entity&) {
            return true;
        }, 0);
        REQUIRE(entities.size() == 1);
    }
    {
        auto entities = entityRoot->findEntities([](const Entity&) {
            return false;
        }, 0);
        REQUIRE(entities.size() == 0);
    }
    {
        auto entities = entityRoot->findEntities([](const Entity&) {
            return true;
        }, 1);
        REQUIRE(entities.size() == 2);
    }
    {
        auto entities = entityRoot->findEntities([](const Entity&) {
            return false;
        }, 1);
        REQUIRE(entities.size() == 0);
    }
    {
        auto entities = entityRoot->findEntities(0);
        REQUIRE(entities.size() == 1);
    }
    {
        auto entities = entityRoot->findEntities(1);
        REQUIRE(entities.size() == 2);
    }
}
