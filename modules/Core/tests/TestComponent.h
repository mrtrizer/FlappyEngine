#pragma once

#include <Component.h>

#include "TestManager.h"

class TestComponent: public flappy::Component {
public:
    class IMock {
    public:
        virtual void init() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
    };

    TestComponent(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {}

    void init() override {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }

    void update(flappy::DeltaTime dt) override {
        if (m_mockComponent != nullptr)
            m_mockComponent->update(dt);
    }

    void testManager() {
        manager<TestManager>()->test();
    }

private:
    IMock* m_mockComponent;
};

class TestComponentEmpty: public flappy::Component {};
