#include "Box2DPolygonComponent.h"

#include "Box2DWorldManager.h"
#include "Box2DUtils.h"

namespace flappy {

Box2DPolygonComponent::Box2DPolygonComponent(Handle<Hierarchy> hierarchy)
    : m_worldManager(hierarchy->manager<Box2DWorldManager>())
{}

void Box2DPolygonComponent::setVertices(const std::vector<glm::vec2>& vertices) {
    m_vertices = vertices;
    float sizeFactor = m_worldManager->sizeFactor();
    auto b2Vertices = glmVec2ToB2Vec2(vertices, sizeFactor);
    auto shape = std::make_shared<b2PolygonShape>();
    shape->Set(b2Vertices.data(), b2Vertices.size());
    setShape(shape);
}

}
