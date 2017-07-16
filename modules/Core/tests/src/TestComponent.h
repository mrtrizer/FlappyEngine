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
    {}

    void update(flappy::DeltaTime dt) override
    {
        if (m_mockComponent != nullptr)
            m_mockComponent->update(dt);
    }

protected:

    void init() override
    {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }

    void deinit() override
    {
        if (m_mockComponent != nullptr)
            m_mockComponent->deinit();
    }

    void testManager()
    {
        manager<TestManager>()->test();
    }

private:
    IMock* m_mockComponent;
};

class TestComponentEmpty: public flappy::Component {};
