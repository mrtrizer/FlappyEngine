#pragma once

#include <vector>

#include "Box2DFixtureComponent.h"

namespace flappy {

class Box2DChainComponent: public Box2DFixtureComponent {
public:
    Box2DChainComponent();

    void setVertices(std::vector<glm::vec2> vertices);
    std::vector<glm::vec2> vertices() { return m_vertices; }

private:
    std::vector<glm::vec2> m_vertices;

    void init(std::vector<glm::vec2> vertices);
};

} // flappy
