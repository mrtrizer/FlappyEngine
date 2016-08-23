#include "catch.hpp"
#include "fakeit.hpp"

#include <memory>

#include <core/component.h>
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

TEST_CASE( "Component::update()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,update));

    auto component = make_shared<TestComponent>(&mock.get());

    component->update(1);

    Verify(Method(mock,update).Using(1)).Exactly(1);
}

TEST_CASE( "Component::init()") {
    Mock<TestComponent::IMock> mock;
    Fake(Method(mock,init));

    auto component = make_shared<TestComponent>(&mock.get());

    component->init();

    Verify(Method(mock,init)).Exactly(1);
}

TEST_CASE( "Component::entity()" ) {
    auto flappyApp = std::make_shared<FlappyApp>();
    auto entity = std::make_shared<Entity>(flappyApp);
    entity->add<TestComponent>();
    REQUIRE(entity->get<TestComponent>()->entity() == entity);
}