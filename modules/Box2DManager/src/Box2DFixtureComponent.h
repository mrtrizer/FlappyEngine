#pragma once

#include <Box2D/Box2D.h>

#include <Component.h>

namespace flappy {

class Box2DFixtureComponent: public Component<Box2DFixtureComponent> {
public:
    Box2DFixtureComponent();

    float friction() const { return m_friction; }
    void setFriction(float friction);

    float elasticity() const { return m_elasticity; }
    void setElasticity(float elasticity);

    float density() const { return m_density; }
    void setDensity(float density);

    bool isSensor() const { return m_isSensor; }
    void setIsSensor(bool isSensor);

    uint16_t categoryBits() const { return m_categoryBits; }
    void setCategoryBits(const uint16_t &categoryBits);

    uint16_t maskBits() const { return m_maskBits; }
    void setMaskBits(const uint16_t &maskBits);

    int16_t groupIndex() const { return m_groupIndex; }
    void setGroupIndex(const int16_t &groupIndex);

protected:
    void setShape(std::shared_ptr<b2Shape> shape);

private:
    void deinitFixture(b2Fixture *fixture);
    b2Fixture *initFixture(std::shared_ptr<b2Shape> shape);

    std::shared_ptr<b2Shape> m_shape;

    float m_friction = 0.2f;
    float m_elasticity = 0.0f;
    float m_density = 0.0f;
    bool m_isSensor = false;

    uint16_t m_categoryBits = 0x0001;
    uint16_t m_maskBits = 0xFFFF;
    int16_t m_groupIndex = 0;

    b2Fixture* m_fixture = nullptr;
};

} // flappy
