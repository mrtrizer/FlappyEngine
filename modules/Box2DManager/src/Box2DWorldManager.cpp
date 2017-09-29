#include "Box2DWorldManager.h"

namespace flappy {

Box2DWorldManager::Box2DWorldManager():m_world(b2Vec2(0.0f, -9.8f))
{

    events()->subscribe([this](UpdateEvent e) {
        update(e.dt);
    });
}

void Box2DWorldManager::update(DeltaTime dt) {
    m_world.Step((float32)dt, (int32)m_velocityIterations, (int32)m_positionIterations);
}

void Box2DWorldManager::setGravity(glm::vec2 gravity) {
    m_world.SetGravity(b2Vec2(gravity.x, gravity.y));
}

float Box2DWorldManager::sizeFactor() const
{
    return m_sizeFactor;
}

void Box2DWorldManager::setSizeFactor(float sizeFactor)
{
    m_sizeFactor = sizeFactor;
    events()->post(Box2DWorldScaleChanged());
}

int Box2DWorldManager::velocityIterations() const
{
    return m_velocityIterations;
}

void Box2DWorldManager::setVelocityIterations(int velocityIterations)
{
    m_velocityIterations = velocityIterations;
}

int Box2DWorldManager::positionIterations() const
{
    return m_positionIterations;
}

void Box2DWorldManager::setPositionIterations(int positionIterations)
{
    m_positionIterations = positionIterations;
}


} // flappy
