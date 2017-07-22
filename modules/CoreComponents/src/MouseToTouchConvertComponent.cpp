#include "MouseToTouchConvertComponent.h"

#include "TouchInputManager.h"
#include "MouseInputManager.h"

namespace flappy {

MouseToTouchConvertComponent::MouseToTouchConvertComponent()
    :Component({TouchInputManager::id(), MouseInputManager::id()})
{}

void MouseToTouchConvertComponent::init()
{
    m_mouseDownSubscription = events()->subscribeIn([this](MouseInputManager::MouseDownEvent e){
        if (e.button == MouseInputManager::MouseButton::LEFT)
            manager<TouchInputManager>()->setTouchDown(e.pos, 0);
    });

    m_mouseUpSubscription = events()->subscribeIn([this](MouseInputManager::MouseUpEvent e){
        if (e.button == MouseInputManager::MouseButton::LEFT)
            manager<TouchInputManager>()->setTouchUp(e.pos, 0);
    });

    m_mouseMoveSubscription = events()->subscribeIn([this](MouseInputManager::MouseMoveEvent e){
        if (manager<MouseInputManager>()->isMousePressed(MouseInputManager::MouseButton::LEFT))
            manager<TouchInputManager>()->setTouchPos(e.pos, 0);
    });
}

void MouseToTouchConvertComponent::deinit()
{
    events()->unsubscribe(m_mouseDownSubscription);
    events()->unsubscribe(m_mouseUpSubscription);
    events()->unsubscribe(m_mouseMoveSubscription);
}

} // flappy
