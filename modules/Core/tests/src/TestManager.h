#pragma once

#include <Manager.h>

class TestManager: public flappy::Manager<TestManager>
{
public:
    class IMock
    {
    public:
        virtual void init() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
        virtual void test() = 0;
    };

    TestManager(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {
        subscribe([this](InitEvent) {
            if (m_mockComponent != nullptr)
                m_mockComponent->init();
        });

        subscribe([this](UpdateEvent e) {
            if (m_mockComponent != nullptr)
                m_mockComponent->update(e.dt);
        });
    }

    void test()
    {
        if (m_mockComponent != nullptr)
            m_mockComponent->test();
    }

private:
    IMock* m_mockComponent;
};

class TestManagerEmpty1: public flappy::Manager<TestManagerEmpty1> {};
class TestManagerEmpty2: public flappy::Manager<TestManagerEmpty2> {};
class TestManagerOverride: public flappy::Manager<TestManagerOverride> {};

class TestManagerDependence: public flappy::Manager<TestManagerDependence> {};
class TestManagerDependent: public flappy::Manager<TestManagerDependent> {};
