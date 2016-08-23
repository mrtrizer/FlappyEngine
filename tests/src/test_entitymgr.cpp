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

class TestComponentEmpty: public Component {

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

TEST_CASE( "EntityMgr::remove()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    REQUIRE(entityMgr.entities().size() == 1);
    entityMgr.remove(entity1);
    entityMgr.update(1);
    REQUIRE(entityMgr.entities().size() == 0);
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
    REQUIRE(entityMgr.find([](EP){return true;}) == nullptr);
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    auto entity3 = entityMgr.create();
    REQUIRE(entityMgr.find([entity2](EP e){return e == entity2;}) == entity2);
    entityMgr.remove(entity3);
    entityMgr.update(1);
    REQUIRE(entityMgr.find([entity3](EP e){return e == entity3;}) == nullptr);
}

TEST_CASE( "EntityMgr::findAll()") {
    EntityMgr entityMgr;
    REQUIRE(entityMgr.findAll([](EP){return true;}).size() == 0);
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    REQUIRE(entityMgr.entities().size() == 2);
    REQUIRE(entityMgr.findAll([](EP){return true;}).size() == 2);
    REQUIRE(entityMgr.findAll([](EP){return false;}).size() == 0);
}

TEST_CASE( "EntityMgr::entities()") {
    EntityMgr entityMgr;
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    REQUIRE(entityMgr.entities().size() == 2);
    REQUIRE(entityMgr.entities().back() == entity2);
    REQUIRE(entityMgr.entities().front() == entity1);
}

TEST_CASE( "EntityMgr::each()") {
    EntityMgr entityMgr;
    unsigned componentCount = 0;
    entityMgr.each<TestComponent>([&componentCount](EP){
        componentCount++;
    });
    REQUIRE(componentCount == 0);
    auto entity1 = entityMgr.create();
    auto entity2 = entityMgr.create();
    auto entity3 = entityMgr.create();
    auto entity4 = entityMgr.create();
    entity1->add<TestComponent>();
    entity2->add<TestComponent>();
    entity3->add<TestComponentEmpty>();
    componentCount = 0;
    entityMgr.each<TestComponent>([&componentCount](EP){
        componentCount++;
    });
    REQUIRE(componentCount == 2);
}
