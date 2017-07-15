#include "Box2DWorldManager.h"

namespace flappy {

Box2DWorldManager::Box2DWorldManager():m_world(b2Vec2(0.0, -9.8))
{

}

void Box2DWorldManager::update(DeltaTime dt) {
    m_world.Step((float32)dt, m_velocityIterations, m_positionIterations);
}

} // flappy
