#include "Box2DRayCastCallback.h"

namespace flappy {

Box2DRayCastCallback::Box2DRayCastCallback()
{}

float32 Box2DRayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
{
    m_fixture = fixture;
    m_point = point;
    m_normal = normal;
    m_fraction = fraction;
    return fraction;
}

} // flappy
