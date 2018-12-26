#include "Box2DCircleComponent.h"

#include "Box2DWorldManager.h"

namespace flappy {

Box2DCircleComponent::Box2DCircleComponent(Handle<Hierarchy> hierarchy)
    : m_worldManager(hierarchy->manager<Box2DWorldManager>())
{}

void Box2DCircleComponent::setRadius(float radius) {
    m_radius = radius;
    float sizeFactor = m_worldManager->sizeFactor();
    auto shape = std::make_shared<b2CircleShape>();
    shape->m_radius = radius * sizeFactor;
    setShape(shape);
}

}
