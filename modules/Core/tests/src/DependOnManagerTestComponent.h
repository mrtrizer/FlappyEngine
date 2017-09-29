#pragma once

#include <Component.h>

#include "TestManager.h"

class DependOnManagerTestComponent: public flappy::Component<DependOnManagerTestComponent> {
public:
    class IMock
    {
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
    };

    DependOnManagerTestComponent(IMock* mockComponent = nullptr)
        : m_mockComponent (mockComponent)
    {
            addDependency(TestManager::id());

            subscribe([this](InitEvent) {
                if (m_mockComponent != nullptr)
                    m_mockComponent->init();
            });

            subscribe([this](DeinitEvent) {
                if (m_mockComponent != nullptr)
                    m_mockComponent->deinit();
            });

    }

private:
    IMock* m_mockComponent;
};
