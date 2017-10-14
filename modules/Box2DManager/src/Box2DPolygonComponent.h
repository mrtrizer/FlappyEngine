#pragma once

#include "Box2DFixtureComponent.h"

namespace flappy {

class Box2DPolygonComponent: public Box2DFixtureComponent {
public:
    Box2DPolygonComponent();

    void setSize(glm::vec2 size);
    glm::vec2 size() { return m_size; }

private:
    glm::vec2 m_size;

    void init(glm::vec2 size);
};

} // flappy
