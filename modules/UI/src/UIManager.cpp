#include "UIManager.h"

#include <TouchInputManager.h>
#include <TransformComponent.h>
#include <Hierarchy.hpp>

#include "TouchComponent.h"

namespace flappy {

UIManager::UIManager(Handle<Hierarchy> hierarchy)
{
    auto touchInputManager = hierarchy->manager<TouchInputManager>();
    m_downSubscription = touchInputManager->eventBus().subscribe([this](TouchInputManager::TouchDownEvent e) {
        auto touchComponent = findTouchComponentInPoint(e.pos);
        if (touchComponent != nullptr)
            touchComponent->touchDown(e.pos, e.index);
    });
    m_upSubscription = touchInputManager->eventBus().subscribe([this](TouchInputManager::TouchUpEvent e) {
        auto touchComponent = findTouchComponentInPoint(e.pos);
        if (touchComponent != nullptr)
            touchComponent->touchUp(e.pos, e.index);
    });
    m_moveSubscription = touchInputManager->eventBus().subscribe([this](TouchInputManager::TouchMoveEvent e) {
        auto touchComponent = findTouchComponentInPoint(e.pos);
        if (touchComponent != nullptr)
            touchComponent->touchMove(e.pos, e.index);
    });
}

Handle<TouchComponent> UIManager::findTouchComponentInPoint(glm::vec2 point)
{
    for (auto touchComponent : m_touchComponentList) {
        if (touchComponent->testPoint(point))
            return touchComponent;
    }
    return Handle<TouchComponent>();
}

void UIManager::registerTouchComponent(Handle<TouchComponent> touchComponent)
{
    m_touchComponentList.push_back(touchComponent);
}

void UIManager::unregisterTouchComponent(Handle<TouchComponent> touchComponent)
{
    m_touchComponentList.remove(touchComponent);
}

} // flappy
