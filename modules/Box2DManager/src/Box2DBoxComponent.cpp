#include "Box2DBoxComponent.h"

#include "Box2DBodyManager.h"
#include "Box2DWorldManager.h"

namespace flappy {

Box2DBoxComponent::Box2DBoxComponent(Handle<Hierarchy> hierarchy)
    : m_box2DWorldManager(hierarchy->manager<Box2DWorldManager>())
{}

void Box2DBoxComponent::init(glm::vec2 size) {
    float sizeFactor = m_box2DWorldManager->sizeFactor();
    auto dynamicBox = std::make_shared<b2PolygonShape>();
    dynamicBox->SetAsBox(size.x * sizeFactor * 0.5f, size.y * sizeFactor * 0.5f);
    setShape(dynamicBox);
}

void Box2DBoxComponent::setSize(glm::vec2 size) {
    m_size = size;
    init(size);
}

}
