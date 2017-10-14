#pragma once

#include "Box2DFixtureComponent.h"

namespace flappy {

class Box2DCircleComponent: public Box2DFixtureComponent {
public:
    Box2DCircleComponent();

    void setRadius(float radius);
    float radius() { return m_radius; }

private:
    float m_radius;

    void init(float radius);
};

} // flappy
