#include "Box2DPolygonComponent.h"

#include "Box2DBodyComponent.h"

namespace flappy {

Box2DPolygonComponent::Box2DPolygonComponent() {
    addDependency(Box2DBodyComponent::id());

    events()->subscribe([this](InitEvent) {
        auto dynamicBox = std::make_shared<b2PolygonShape>();
        dynamicBox->SetAsBox(m_size.x, m_size.y);
        setShape(dynamicBox);
    });
}

void Box2DPolygonComponent::setSize(glm::vec2 size) {
    m_size = size;
    auto dynamicBox = std::make_shared<b2PolygonShape>();
    dynamicBox->SetAsBox(size.x, size.y);
    setShape(dynamicBox);
}

}
