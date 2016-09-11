#pragma once

#include <core/component.h>

class TestComponent: public flappy::Component {
public:
    class IMock {
    public:
        virtual void init() = 0;
        virtual void update(flappy::TimeDelta dt) = 0;
    };

    TestComponent(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {}

    void init() override {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }

    void update(flappy::TimeDelta dt) override {
        if (m_mockComponent != nullptr)
            m_mockComponent->update(dt);
    }

private:
    IMock* m_mockComponent;
};

class TestComponentEmpty: public flappy::Component {};
