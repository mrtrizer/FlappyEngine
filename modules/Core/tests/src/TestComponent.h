#pragma once

#include <Component.h>

#include "TestManager.h"

class TestComponent: public flappy::Component<TestComponent>
{
public:
    class IMock
    {
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
    };

    TestComponent(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {
        subscribe([this](InitEvent) {
            if (m_mockComponent != nullptr)
                m_mockComponent->init();
        });

        subscribe([this](DeinitEvent) {
            if (m_mockComponent != nullptr)
                m_mockComponent->deinit();
        });

        subscribe([this](UpdateEvent e) {
            if (m_mockComponent != nullptr)
                m_mockComponent->update(e.dt);
        });

    }


protected:

    void testManager()
    {
        manager<TestManager>()->test();
    }

private:
    IMock* m_mockComponent;
};

class TestComponentEmpty: public flappy::Component<TestComponentEmpty> {};
