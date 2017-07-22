#include "TouchComponent.h"

#include <Entity.h>
#include <Box2DBodyComponent.h>

#include "UIManager.h"

namespace flappy {

TouchComponent::TouchComponent() : Component({UIManager::id()}, {Box2DBodyComponent::id()})
{}

void TouchComponent::touchDown(glm::vec2 pos, int index)
{

}

void TouchComponent::touchUp(glm::vec2 pos, int index)
{

}

void TouchComponent::touchMove(glm::vec2 pos, int index)
{

}

bool TouchComponent::testPoint(glm::vec2 pos)
{
    entity()->component<Box2DBodyComponent>()->testPoint(pos);
}

void TouchComponent::init()
{
    manager<UIManager>()->registerTouchComponent(selfPointer<TouchComponent>());
}

void TouchComponent::deinit()
{
    manager<UIManager>()->unregisterTouchComponent(selfPointer<TouchComponent>());
}

} // flappy
