#include "UIManager.h"

#include <TouchInputManager.h>
#include <TransformComponent.h>

#include "TouchComponent.h"

namespace flappy {

UIManager::UIManager()
{
    events()->subscribe([this](TouchInputManager::TouchDownEvent e) {
        auto touchComponent = findTouchComponentInPoint(e.pos);
        if (touchComponent != nullptr)
            touchComponent->touchDown(e.pos, e.index);
    });
    events()->subscribe([this](TouchInputManager::TouchUpEvent e) {
        auto touchComponent = findTouchComponentInPoint(e.pos);
        if (touchComponent != nullptr)
            touchComponent->touchUp(e.pos, e.index);
    });
    events()->subscribe([this](TouchInputManager::TouchMoveEvent e) {
        auto touchComponent = findTouchComponentInPoint(e.pos);
        if (touchComponent != nullptr)
            touchComponent->touchMove(e.pos, e.index);
    });
}

SafePtr<TouchComponent> UIManager::findTouchComponentInPoint(glm::vec2 point)
{
    for (auto touchComponent : m_touchComponentList) {
        if (touchComponent->testPoint(point))
            return touchComponent;
    }
    return SafePtr<TouchComponent>();
}

void UIManager::registerTouchComponent(SafePtr<TouchComponent> touchComponent)
{
    m_touchComponentList.push_back(touchComponent);
}

void UIManager::unregisterTouchComponent(SafePtr<TouchComponent> touchComponent)
{
    m_touchComponentList.remove(touchComponent);
}

} // flappy
