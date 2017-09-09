#include "MouseToTouchConvertComponent.h"

#include "TouchInputManager.h"
#include "MouseInputManager.h"

namespace flappy {

MouseToTouchConvertComponent::MouseToTouchConvertComponent()
{
    addDependency(TouchInputManager::id());
    addDependency(MouseInputManager::id());

    subscribe([this](InitEvent) {
        m_mouseDownSubscription = subscribe([this](MouseInputManager::MouseDownEvent e){
            if (e.button == MouseInputManager::MouseButton::LEFT)
                manager<TouchInputManager>()->setTouchDown(e.pos, 0);
        });

        m_mouseUpSubscription = subscribe([this](MouseInputManager::MouseUpEvent e){
            if (e.button == MouseInputManager::MouseButton::LEFT)
                manager<TouchInputManager>()->setTouchUp(e.pos, 0);
        });

        m_mouseMoveSubscription = subscribe([this](MouseInputManager::MouseMoveEvent e){
            if (manager<MouseInputManager>()->isMousePressed(MouseInputManager::MouseButton::LEFT))
                manager<TouchInputManager>()->setTouchPos(e.pos, 0);
        });
    });

    subscribe([this](DeinitEvent) {
        events()->unsubscribe(m_mouseDownSubscription);
        events()->unsubscribe(m_mouseUpSubscription);
        events()->unsubscribe(m_mouseMoveSubscription);
    });
}

} // flappy
