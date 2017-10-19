#pragma once

#include <Box2D.h>

namespace flappy {

class Box2DRayCastCallback: public b2RayCastCallback {
public:
    Box2DRayCastCallback();

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) final;

    b2Fixture* fixture() { return m_fixture; }
    b2Vec2 point() { return m_point; }
    b2Vec2 normal() { return m_normal; }
    float32 fraction() { return m_fraction; }

private:
    b2Fixture* m_fixture = nullptr;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float32 m_fraction;
};

} // flappy
