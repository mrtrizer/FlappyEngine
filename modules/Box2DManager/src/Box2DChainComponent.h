#pragma once

#include <vector>

#include <Handle.hpp>

#include "Box2DFixtureComponent.h"

namespace flappy {

class Hierarchy;
class Box2DWorldManager;

class Box2DChainComponent: public Box2DFixtureComponent {
public:
    Box2DChainComponent(Handle<Hierarchy> hierarchy);

    void setVertices(const std::vector<glm::vec2>& vertices);
    const std::vector<glm::vec2>& vertices() const { return m_vertices; }

private:
    std::vector<glm::vec2> m_vertices = {{0.0f, 0.0f}, {1.0f, 0.0f}};
    Handle<Box2DWorldManager> m_worldManager;
};

} // flappy
