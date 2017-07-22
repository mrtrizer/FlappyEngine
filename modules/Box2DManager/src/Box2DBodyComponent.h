#pragma once

#include <Component.h>

#include <glm/vec3.hpp>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DBodyComponent: public Component<Box2DBodyComponent> {
public:
    Box2DBodyComponent();

    b2Body& body() {
        if (m_body == nullptr)
            throw std::runtime_error("Box2DWorldManager is not found in hierarchy.");
        return *m_body;
    }

    bool testPoint(glm::vec2 point);

private:
    void init() final;
    void deinit() final;
    void update(DeltaTime dt) final;

    b2Body* m_body;
    glm::vec3 m_lastTransformPos;
    float m_lastTransformAngle;
};

} // flappy
