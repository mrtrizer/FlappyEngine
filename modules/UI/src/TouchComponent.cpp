#include "TouchComponent.h"

#include <Hierarchy.hpp>
#include <Box2DBodyManager.h>

#include "UIManager.h"

namespace flappy {

TouchComponent::TouchComponent(Handle<Entity> entity)
    : m_uiManager(entity->hierarchy()->manager<UIManager>())
    , m_box2dBodyComponent(entity->component<Box2DBodyManager>())
{
    m_uiManager->registerTouchComponent(selfHandle());
}
    
TouchComponent::~TouchComponent() {
    m_uiManager->unregisterTouchComponent(selfHandle());
}

void TouchComponent::touchDown(glm::vec2 pos, int index)
{
    m_eventBus.post(TouchDownEvent(pos, index));
}

void TouchComponent::touchUp(glm::vec2 pos, int index)
{
    m_eventBus.post(TouchUpEvent(pos, index));
}

void TouchComponent::touchMove(glm::vec2 pos, int index)
{
    m_eventBus.post(TouchMoveEvent(pos, index));
}

bool TouchComponent::testPoint(glm::vec2 pos)
{
    return m_box2dBodyComponent->testPoint(pos);
}

} // flappy
