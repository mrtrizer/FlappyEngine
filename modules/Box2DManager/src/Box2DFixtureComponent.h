#pragma once

#include <Box2D/Box2D.h>

#include <Component.h>

namespace flappy {

class Box2DFixtureComponent: public Component<Box2DFixtureComponent> {
public:
    Box2DFixtureComponent();

    void setShape(const b2Shape &shape);

    float friction() const;
    void setFriction(float friction);

    float elasticity() const;
    void setElasticity(float elasticity);

    float density() const;
    void setDensity(float density);

    bool isSensor() const;
    void setIsSensor(bool isSensor);

    uint16_t categoryBits() const;
    void setCategoryBits(const uint16_t &categoryBits);

    uint16_t maskBits() const;
    void setMaskBits(const uint16_t &maskBits);

    int16_t groupIndex() const;
    void setGroupIndex(const int16_t &groupIndex);

    b2Fixture* fixture() const;

private:
    void init() override;
    void deinit() override;

    void initFixture();

    const b2Shape* m_shape;

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
