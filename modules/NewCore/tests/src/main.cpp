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
        m_something = something;
    }

    int something() { return m_something; }

    void update(float dt) {
        m_entity->component<OtherTestComponent>()->move(dt);
    }

private:
    Handle<Entity> m_entity;
    int m_something = 0;
};

//EntityBlueprint createButton() {
//    // ...
//}

class ITestManager : public IManager {
public:
    virtual void setSomething(int something) = 0;
    virtual int something() = 0;
};

class TestManager : public ITestManager {
public:
    TestManager(Handle<Hierarchy>) {

    }

    void setSomething(int something) override { m_something = something; }
    int something() override { return m_something; }

    void update(float) override {

    }

private:
    int m_something = 0;
};

TEST_CASE( "Hierarchy") {
    auto hierarchy = Heap::create<Hierarchy>();
    //{
    auto testManager = hierarchy->initManager<ITestManager, TestManager>();
    testManager->setSomething(200);

    auto entity1 = hierarchy->rootEntity()->createEntity();
    entity1->component<TestComponent>()->setSomething(100);
    //entity1->insertSubtree(createButton());

    REQUIRE(entity1->findComponent<TestComponent>()->something() == 100);
    REQUIRE(hierarchy->manager<ITestManager>()->something() == 200);

    hierarchy->update(1.0f);
    //}
}
