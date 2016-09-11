#pragma once

#include <core/manager.h>

class TestManager: public flappy::Manager<TestManager> {
public:
    class IMock {
    public:
        virtual void init() = 0;
        virtual void update(flappy::TimeDelta dt) = 0;
        virtual void test() = 0;
    };

    TestManager(IMock* mockComponent = nullptr):
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

    void test() {
        if (m_mockComponent != nullptr)
            m_mockComponent->test();
    }

private:
    IMock* m_mockComponent;
};

class TestManagerEmpty1: public flappy::Manager<TestManagerEmpty1> {};
class TestManagerEmpty2: public flappy::Manager<TestManagerEmpty2> {};
class TestManagerOverride: public flappy::Manager<TestManager> {};
