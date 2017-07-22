#include "TouchComponent.h"

#include <Box2DBodyComponent.h>

#include "UIManager.h"

namespace flappy {

TouchComponent::TouchComponent() : Component({UIManager::id()}, {Box2DBodyComponent::id()})
{

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
