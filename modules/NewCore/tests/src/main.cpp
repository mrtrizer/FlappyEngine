#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <Hierarchy.hpp>
#include <Heap.hpp>

using namespace fakeit;
using namespace std;

class OtherTestComponet {
public:
    void move(float dt) {

    }
};

class TestComponent : public PutAfter<OtherTestComponet> {
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

HierarchyTemplate createButton() {
    // ...
}

int main(char** argv, int argc) {
    auto hierarchy = Heap::create<Hierarchy>();
    auto testManager = hierarchy->initManager<ITestManager, TestManager>();
    testManager->setSomething(100);

    auto appManager = hierarchy->initManager<AppManager>();
    appManager->setArgs(argv, argc);

    auto entity1 = hierarchy->rootEntity()->createEntity();
    entity1->component<TestComponent>()->setSomething(100);
    entity1->insertSubtree(createButton());

    hierarchy->update(1.0f);
}

TEST_CASE( "Hierarchy") {


}
