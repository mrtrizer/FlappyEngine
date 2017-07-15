#pragma once

#include <Manager.h>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DWorldManager: public Manager<Box2DWorldManager> {
public:
    Box2DWorldManager();

    b2World world() { return m_world; }

    void update(DeltaTime dt);

    void setVelocityIterations(int value) { m_velocityIterations = value; }
    void setPositionIterations(int value) { m_positionIterations = value; }

    int velocityIterations() { return m_velocityIterations; }
    int positionIterations() { return m_positionIterations; }

private:
    b2World m_world;
    int32 m_velocityIterations = 6;
    int32 m_positionIterations = 2;
};

} // flappy
