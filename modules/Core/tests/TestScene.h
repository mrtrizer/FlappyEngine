#pragma once

#include <Scene.h>

class TestScene: public flappy::Scene {
public:
    class IMock {
    public:
        virtual void init() = 0;
    };

    TestScene(IMock* m_mock = nullptr):
        m_mock (m_mock)
    {}

    void init() override {
        if (m_mock != nullptr)
            m_mock->init();
    }

private:
    IMock* m_mock;
};
