#pragma once

#include "Box2DFixtureComponent.h"

namespace flappy {

class Box2DWorldManager;
class Hierarchy;

class Box2DPolygonComponent: public Box2DFixtureComponent {
public:
    Box2DPolygonComponent(Handle<Hierarchy> hierarchy);

    void setVertices(const std::vector<glm::vec2>& vertices);
    const std::vector<glm::vec2>& vertices() { return m_vertices; }

private:
    std::vector<glm::vec2> m_vertices;
    Handle<Box2DWorldManager> m_worldManager;
};

} // flappy
