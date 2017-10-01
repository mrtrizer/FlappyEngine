#pragma once

#include <Component.h>
#include <TouchComponent.h>

class ButtonListenerComponent: public flappy::Component<ButtonListenerComponent>
{
public:
    class IMock
    {
    public:
        virtual void onTouchDown(glm::vec2 pos, int index) = 0;
        virtual void onTouchUp(glm::vec2 pos, int index) = 0;
        virtual void onTouchMove(glm::vec2 pos, int index) = 0;
    };

    ButtonListenerComponent(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {

        subscribe([this](const flappy::TouchComponent::TouchDownEvent& e) {
            if (m_mockComponent != nullptr)
                m_mockComponent->onTouchDown(e.pos, e.index);
        });

        subscribe([this](const flappy::TouchComponent::TouchUpEvent& e) {
            if (m_mockComponent != nullptr)
                m_mockComponent->onTouchUp(e.pos, e.index);
        });

        subscribe([this](const flappy::TouchComponent::TouchMoveEvent& e) {
            if (m_mockComponent != nullptr)
                m_mockComponent->onTouchMove(e.pos, e.index);
        });

    }

private:
    IMock* m_mockComponent;
};
