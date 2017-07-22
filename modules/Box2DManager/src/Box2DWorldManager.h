#pragma once

#include <Manager.h>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DWorldManager: public Manager<Box2DWorldManager> {
public:
    Box2DWorldManager();

    b2World& world() { return m_world; }

    void update(DeltaTime dt) final;

    void setGravity(glm::vec2 gravity);

    float sizeFactor() const;
    void setSizeFactor(float sizeFactor);

    int velocityIterations() const;
    void setVelocityIterations(int velocityIterations);

    int positionIterations() const;
    void setPositionIterations(int positionIterations);

private:
    b2World m_world;
    int m_velocityIterations = 6;
    int m_positionIterations = 2;
    float m_sizeFactor;
};

} // flappy
