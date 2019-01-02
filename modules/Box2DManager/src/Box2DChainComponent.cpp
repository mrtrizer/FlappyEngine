#include "Box2DChainComponent.h"

#include "Box2DWorldManager.h"
#include "Box2DUtils.h"

namespace flappy {

Box2DChainComponent::Box2DChainComponent(Handle<Entity> entity)
    : m_worldManager(entity->hierarchy()->manager<Box2DWorldManager>())
{}

void Box2DChainComponent::setVertices(const std::vector<glm::vec2>& vertices) {
    m_vertices = vertices;
    float sizeFactor = m_worldManager->sizeFactor();
    auto b2Vertices = glmVec2ToB2Vec2(vertices, sizeFactor);
    auto shape = std::make_shared<b2ChainShape>();
    shape->CreateChain(b2Vertices.data(), b2Vertices.size());
    setShape(shape);
}

}
