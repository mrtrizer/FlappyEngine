#include "Box2DChainComponent.h"

#include "Box2DBodyComponent.h"
#include "Box2DWorldManager.h"
#include "Box2DUtils.h"

namespace flappy {

Box2DChainComponent::Box2DChainComponent() {

    addDependency(Box2DBodyComponent::id());
    addDependency(Box2DWorldManager::id());

    events()->subscribe([this](InitEvent) {
        init(m_vertices);
    });
}

void Box2DChainComponent::init(std::vector<glm::vec2> vertices) {
    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();
    auto b2Vertices = glmVec2ToB2Vec2(vertices, sizeFactor);
    auto shape = std::make_shared<b2ChainShape>();
    shape->CreateChain(b2Vertices.data(), b2Vertices.size());
    setShape(shape);
}

void Box2DChainComponent::setVertices(std::vector<glm::vec2> vertices) {
    m_vertices = vertices;
    if (isInitialized()) {
        init(vertices);
    }
}

}
