#include "catch.hpp"
#include "fakeit.hpp"

#include <memory>

#include <core/entitymgr.h>

using namespace flappy;
using namespace fakeit;
using namespace std;

class TestComponent: public Component {
public:
    class IMock {
    public:
        virtual void init() = 0;
        virtual void update(TimeDelta dt) = 0;
    };

    TestComponent(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {}

    void init() override {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }

    void update(TimeDelta dt) override {
        if (m_mockComponent != nullptr)
            m_mockComponent->update(dt);
    }

private:
    IMock* m_mockComponent;
};

TEST_CASE( "EntityMgr::update()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    entity1->add<TestComponent>(&mock.get());
    entityMgr.update(1);

    Verify(Method(mock,init), Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "EntityMgr::reset()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    REQUIRE(entityMgr.entities().size() == 1);
    entityMgr.reset();
    entityMgr.update(1);
    REQUIRE(entityMgr.entities().size() == 0);
}

TEST_CASE( "EntityMgr::create()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    REQUIRE( entityMgr.entities().size() == 1);
}

TEST_CASE( "EntityMgr::create(std::function)") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create([](EP e){
        e->add<TestComponent>();
    });
    REQUIRE(entityMgr.entities().size() == 1);
}

TEST_CASE( "EntityMgr::find()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    auto entity3 = entityMgr.create();
    REQUIRE(entityMgr.find([entity2](EP e){return e == entity2;}) == entity2);
}

TEST_CASE( "EntityMgr::findAll()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    REQUIRE(entityMgr.entities().size() == 2);
    REQUIRE(entityMgr.findAll([](EP){return true;}).size() == 2);
    REQUIRE(entityMgr.findAll([](EP){return false;}).size() == 0);
}

TEST_CASE( "EntityMgr::each()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    auto entity3 = entityMgr.create();
    entity1->add<TestComponent>();
    entity2->add<TestComponent>();
    unsigned componentCount = 0;
    entityMgr.each<TestComponent>([&componentCount](EP){
        componentCount++;
    });
    REQUIRE(componentCount == 2);
}
