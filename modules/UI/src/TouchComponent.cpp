#include "TouchComponent.h"

#include <Entity.h>
#include <Box2DBodyManager.h>

#include "UIManager.h"

namespace flappy {

TouchComponent::TouchComponent()
{
    addDependency(UIManager::id());
    addDependency(Box2DBodyManager::id());

    events()->subscribe([this](InitEvent) {
        manager<UIManager>()->registerTouchComponent(selfPointer());
    });

    events()->subscribe([this](DeinitEvent) {
        manager<UIManager>()->unregisterTouchComponent(selfPointer());
    });
}

void TouchComponent::touchDown(glm::vec2 pos, int index)
{
    entity()->events()->post(TouchDownEvent(pos, index));
}

void TouchComponent::touchUp(glm::vec2 pos, int index)
{
    entity()->events()->post(TouchUpEvent(pos, index));
}

void TouchComponent::touchMove(glm::vec2 pos, int index)
{
    entity()->events()->post(TouchMoveEvent(pos, index));
}

bool TouchComponent::testPoint(glm::vec2 pos)
{
    return entity()->manager<Box2DBodyManager>()->testPoint(pos);
}

} // flappy
