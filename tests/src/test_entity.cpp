#include "catch.hpp"
#include "fakeit.hpp"

#include <memory>

#include <core/entity.h>
#include <core/transform.h>
#include <core/flappyapp.h>

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

TEST_CASE( "Entity::add()" ) {
    auto flappyApp = std::make_shared<FlappyApp>();
    auto entity = std::make_shared<Entity>(flappyApp);
    auto component = entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>() == component);
    entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().size() == 2);
}

TEST_CASE( "Entity::get()") {
    auto flappyApp = std::make_shared<FlappyApp>();
    auto entity = std::make_shared<Entity>(flappyApp);
    auto component = entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>() == component);
    entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>() == component); // should return first component
}

TEST_CASE( "Entity::getAll()") {
    auto flappyApp = std::make_shared<FlappyApp>();
    auto entity = std::make_shared<Entity>(flappyApp);
    auto component1 = entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().front() == component1);
    auto component2 = entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().back() == component2);
    auto component3 = entity->add<TestComponent>();
    REQUIRE(entity->getAll<TestComponent>().back() == component3);
    REQUIRE(entity->getAll<TestComponent>().size() == 3);
}

TEST_CASE( "Entity::update()") {
    auto flappyApp = std::make_shared<FlappyApp>();
    auto entity = std::make_shared<Entity>(flappyApp);
    entity->add<TestComponent>();

    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    entity->add<TestComponent>(&mock.get());
    entity->update(1);

    Verify(Method(mock,init), Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "Entity::transform()") {
    auto flappyApp = std::make_shared<FlappyApp>();
    auto entity = std::make_shared<Entity>(flappyApp);
    auto transform = entity->add<Transform>();
    entity->transform() == transform;
}
