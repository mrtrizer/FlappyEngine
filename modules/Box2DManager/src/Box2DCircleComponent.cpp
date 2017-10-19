#include "Box2DCircleComponent.h"

#include "Box2DBodyManager.h"
#include "Box2DWorldManager.h"

namespace flappy {

Box2DCircleComponent::Box2DCircleComponent() {

    addDependency(Box2DBodyManager::id());
    addDependency(Box2DWorldManager::id());

    events()->subscribe([this](InitEvent) {
        init(m_radius);
    });
}

void Box2DCircleComponent::init(float radius) {
    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();
    auto shape = std::make_shared<b2CircleShape>();
    shape->m_radius = radius * sizeFactor;
    setShape(shape);
}

void Box2DCircleComponent::setRadius(float radius) {
    m_radius = radius;
    if (isInitialized()) {
        init(radius);
    }
}

}
