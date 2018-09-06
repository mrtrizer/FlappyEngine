#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <Hierarchy.hpp>
#include <Heap.hpp>
#include <Component.hpp>

using namespace fakeit;
using namespace std;
using namespace flappy;

class OtherTestComponent {
public:
    void move(float dt) {

    }
};

class TestComponent : public PutAfter<OtherTestComponent> {
public:
    TestComponent(Handle<Entity> entity)
        : m_entity(entity)
    {

    }

    void setSomething(int something) {

    }

    void update(float dt) {
        m_entity->component<OtherTestComponent>()->move(dt);
    }

private:
    Handle<Entity> m_entity;
};

//EntityBlueprint createButton() {
//    // ...
//}

class ITestManager : public IManager {
public:
    void setSomething(int) {

    }
};

class TestManager : public ITestManager {
public:
    TestManager(Handle<Hierarchy>) {

    }

    void update(float) override {

    }
};

TEST_CASE( "Hierarchy") {
    auto hierarchy = Heap::create<Hierarchy>();
    auto testManager = hierarchy->initManager<ITestManager, TestManager>();
    testManager->setSomething(100);

    auto entity1 = hierarchy->rootEntity()->createEntity();
    entity1->component<TestComponent>()->setSomething(100);
    //entity1->insertSubtree(createButton());

    hierarchy->update(1.0f);
}
