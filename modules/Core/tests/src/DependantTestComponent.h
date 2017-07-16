#pragma once

#include <Component.h>

class DependantTestComponent: public flappy::Component<DependantTestComponent> {
public:
    class IMock
    {
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
    };

    DependantTestComponent(IMock* mockComponent = nullptr):
        Component({flappy::ClassId<Component, TestManager>().id()}),
        m_mockComponent (mockComponent){

    }

    void init() override final {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }

    void deinit() override final {
        if (m_mockComponent != nullptr)
            m_mockComponent->deinit();
    }
private:
    IMock* m_mockComponent;
};
