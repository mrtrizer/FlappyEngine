#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <EntityManager.h>
#include <TransformComponent.h>
#include <ManagerList.h>
#include <EntityManager.h>

#include "TestManager.h"
#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "Component::update()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));

    shared_ptr<Component> testComponent = make_shared<TestComponent>(&mock.get());

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
    auto managerList = std::make_shared<ManagerList>();
    auto entity = std::make_shared<Entity>();
    entity->setManagerList(managerList);
    entity->create<TestComponent>();
    REQUIRE(entity->findComponent<TestComponent>()->entity() == entity);
}

TEST_CASE( "Component::manager()" ) {
    Mock<TestManager::IMock> mock;
    Fake(Method(mock,init));
    Fake(Method(mock,test));

    auto managerList = make_shared<ManagerList>();
    managerList->create<TestManager>(&mock.get());
    managerList->create<EntityManager>();
    managerList->init();
    auto entity = managerList->manager<EntityManager>()->create();
    auto component = entity->create<Component>();
    component->manager<TestManager>()->test();

    Verify(Method(mock, init), Method(mock,test)).Exactly(1);
}
