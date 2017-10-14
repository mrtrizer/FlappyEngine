#include "Box2DPolygonComponent.h"

#include "Box2DBodyComponent.h"
#include "Box2DWorldManager.h"

namespace flappy {

Box2DPolygonComponent::Box2DPolygonComponent() {

    addDependency(Box2DBodyComponent::id());
    addDependency(Box2DWorldManager::id());

    events()->subscribe([this](InitEvent) {
        init(m_size);
    });
}

void Box2DPolygonComponent::init(glm::vec2 size) {
    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();
    auto dynamicBox = std::make_shared<b2PolygonShape>();
    dynamicBox->SetAsBox(size.x * sizeFactor * 0.5f, size.y * sizeFactor * 0.5f);
    setShape(dynamicBox);
}

void Box2DPolygonComponent::setSize(glm::vec2 size) {
    m_size = size;
    if (isInitialized()) {
        init(size);
    }
}

}
