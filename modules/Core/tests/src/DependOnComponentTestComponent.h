#pragma once

#include <Component.h>

#include "TestComponent.h"

class DependOnComponentTestComponent: public flappy::Component<DependOnComponentTestComponent> {
public:
    class IMock
    {
    public:
        virtual void init() = 0;
        virtual void deinit() = 0;
        virtual void update(flappy::DeltaTime dt) = 0;
    };

    DependOnComponentTestComponent(IMock* mockComponent = nullptr)
        : m_mockComponent (mockComponent)
    {
            addDependency(TestComponent::id());

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
