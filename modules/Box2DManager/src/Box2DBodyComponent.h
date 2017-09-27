#pragma once

#include <Component.h>

#include <glm/vec3.hpp>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DBodyComponent: public Component<Box2DBodyComponent> {
public:
    Box2DBodyComponent();

    b2Body& body();

    bool testPoint(glm::vec2 point);

private:
    b2Body* m_body;
    glm::vec3 m_lastTransformPos;
    float m_lastTransformAngle;

    void update(DeltaTime dt);
};

} // flappy
