#pragma once

#include <TouchComponent.h>
#include <Handle.hpp>

class [[component]] ButtonListenerComponent
{
public:
    class IMock
    {
    public:
        virtual void onTouchDown(glm::vec2 pos, int index) = 0;
        virtual void onTouchUp(glm::vec2 pos, int index) = 0;
        virtual void onTouchMove(glm::vec2 pos, int index) = 0;
    };

    void setMock(IMock* mockComponent) {
        m_mockComponent = mockComponent;
    }
    
    void setEntity(flappy::Handle<flappy::Entity> entity)
    {
        auto touchComponent = entity->component<flappy::TouchComponent>();
        m_touchDownSubscription = touchComponent->eventBus().subscribe(
            [this](const flappy::TouchComponent::TouchDownEvent& e) {
                if (m_mockComponent != nullptr)
                    m_mockComponent->onTouchDown(e.pos, e.index);
            });

        m_touchUpSubscription = touchComponent->eventBus().subscribe(
            [this](const flappy::TouchComponent::TouchUpEvent& e) {
                if (m_mockComponent != nullptr)
                    m_mockComponent->onTouchUp(e.pos, e.index);
            });

        m_touchMoveSubscription = touchComponent->eventBus().subscribe(
            [this](const flappy::TouchComponent::TouchMoveEvent& e) {
                if (m_mockComponent != nullptr)
                    m_mockComponent->onTouchMove(e.pos, e.index);
            });
    }

private:
    IMock* m_mockComponent;
    std::shared_ptr<flappy::ISubscription> m_touchDownSubscription;
    std::shared_ptr<flappy::ISubscription> m_touchUpSubscription;
    std::shared_ptr<flappy::ISubscription> m_touchMoveSubscription;
};
