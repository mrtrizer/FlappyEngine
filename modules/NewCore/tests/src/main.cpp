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
        m_position += dt * m_speed;
    }

private:
    static constexpr float m_speed = 2.0f;
    float m_position = 0.0f;
};

class TestComponent : public PutAfter<OtherTestComponent> {
public:
    TestComponent(Handle<Entity> entity)
        : m_entity(entity)
    {}

    void setSomething(int something) {
        m_something = something;
    }

    int something() { return m_something; }

    void update(float dt) {
        m_entity->component<OtherTestComponent>()->move(dt);
        m_updateTime += dt;
    }

    float updateTime() const { return m_updateTime; }

private:
    Handle<Entity> m_entity;
    int m_something = 0;
    float m_updateTime = 0.0f;
};

//EntityBlueprint createButton() {
//    // ...
//}

class ITestManager : public IManager {
public:
    virtual void setSomething(int something) = 0;
    virtual int something() = 0;
    virtual float updateTime() const = 0;
};

class TestManager : public ITestManager {
public:
    TestManager(Handle<Hierarchy>)
    {}

    void setSomething(int something) override { m_something = something; }
    int something() override { return m_something; }

    void update(float dt) override {
        m_updateTime += dt;
    }

    float updateTime() const override { return m_updateTime; }

private:
    int m_something = 0;
    float m_updateTime = 0.0f;
};

TEST_CASE( "Hierarchy") {
    auto hierarchy = Heap::create<Hierarchy>();

    auto testManager = hierarchy->initManager<ITestManager, TestManager>();
    testManager->setSomething(200);

    auto entity1 = hierarchy->rootEntity()->createEntity();
    entity1->component<TestComponent>()->setSomething(100);
    //entity1->insertSubtree(createButton());

    REQUIRE(entity1->findComponent<TestComponent>()->something() == 100);
    REQUIRE(hierarchy->manager<ITestManager>()->something() == 200);

    hierarchy->update(1.0f);

    REQUIRE(entity1->component<TestComponent>()->updateTime() == 1.0f);
    REQUIRE(hierarchy->manager<ITestManager>()->updateTime() == 1.0f);

    hierarchy->update(1.0f);

    REQUIRE(entity1->component<TestComponent>()->updateTime() == 2.0f);
    REQUIRE(hierarchy->manager<ITestManager>()->updateTime() == 2.0f);
}
