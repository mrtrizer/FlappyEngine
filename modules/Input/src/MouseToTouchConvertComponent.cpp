#include "MouseToTouchConvertComponent.h"

#include "TouchInputManager.h"
#include "MouseInputManager.h"

#include <Hierarchy.hpp>

namespace flappy {

MouseToTouchConvertComponent::MouseToTouchConvertComponent(const Handle<Entity>& entity)
{
    auto hierarchy = entity->hierarchy();
    auto eventBus = hierarchy->manager<EventBus>();
    m_mouseDownSubscription = eventBus->subscribe([hierarchy](MouseInputManager::MouseDownEvent e){
        if (e.button == MouseInputManager::MouseButton::LEFT)
            hierarchy->manager<TouchInputManager>()->setTouchDown(e.pos, 0);
    });

    m_mouseUpSubscription = eventBus->subscribe([hierarchy](MouseInputManager::MouseUpEvent e){
        if (e.button == MouseInputManager::MouseButton::LEFT)
            hierarchy->manager<TouchInputManager>()->setTouchUp(e.pos, 0);
    });

    m_mouseMoveSubscription = eventBus->subscribe([hierarchy](MouseInputManager::MouseMoveEvent e){
        if (hierarchy->manager<MouseInputManager>()->isMousePressed(MouseInputManager::MouseButton::LEFT))
            hierarchy->manager<TouchInputManager>()->setTouchPos(e.pos, 0);
    });
}

} // flappy
