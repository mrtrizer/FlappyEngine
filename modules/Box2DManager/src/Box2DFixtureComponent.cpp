#include "Box2DFixtureComponent.h"

#include <Entity.h>

#include "Box2DBodyManager.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

Box2DFixtureComponent::Box2DFixtureComponent() {

    addDependency(Box2DBodyManager::id());

    events()->subscribe([this](InitEvent) {
        if ((m_shape != nullptr))
            m_fixture = initFixture(m_shape);
    });

    events()->subscribe([this](DeinitEvent) {
        deinitFixture(m_fixture);
        m_fixture = nullptr;
    });

}

void Box2DFixtureComponent::deinitFixture(b2Fixture* fixture) {
    if (fixture != nullptr) {
        auto bodyComponent = entity()->manager<Box2DBodyManager>();
        bodyComponent->destroyFixture(fixture);
    }
}

b2Fixture* Box2DFixtureComponent::initFixture(std::shared_ptr<b2Shape> shape) {
    b2FixtureDef fixtureDef;

    fixtureDef.shape = shape.get();

    // physics params
    fixtureDef.friction = m_friction;
    fixtureDef.restitution = m_elasticity;
    fixtureDef.density = m_density;
    fixtureDef.isSensor = m_isSensor;

    // filter
    fixtureDef.filter.categoryBits = m_categoryBits;
    fixtureDef.filter.maskBits = m_maskBits;
    fixtureDef.filter.groupIndex = m_groupIndex;

    //user data
    fixtureDef.userData = (void*)this;

    auto bodyComponent = entity()->manager<Box2DBodyManager>();
    return bodyComponent->createFixture(&fixtureDef);
}

void Box2DFixtureComponent::setShape(std::shared_ptr<b2Shape> shape) {
    m_shape = shape;
    if (isInitialized()) {
        if (m_fixture != nullptr) {
            deinitFixture(m_fixture);
        }
        m_fixture = initFixture(shape);
    }
}

void Box2DFixtureComponent::setGroupIndex(const int16_t &groupIndex)
{
    m_groupIndex = groupIndex;
    if (m_fixture != nullptr) {
        auto filterData = m_fixture->GetFilterData();
        filterData.groupIndex = groupIndex;
        m_fixture->SetFilterData(filterData);
    }
}

void Box2DFixtureComponent::setMaskBits(const uint16_t &maskBits)
{
    m_maskBits = maskBits;
    if (m_fixture != nullptr) {
        auto filterData = m_fixture->GetFilterData();
        filterData.maskBits = maskBits;
        m_fixture->SetFilterData(filterData);
    }
}

void Box2DFixtureComponent::setCategoryBits(const uint16_t &categoryBits)
{
    m_categoryBits = categoryBits;
    if (m_fixture != nullptr) {
        auto filterData = m_fixture->GetFilterData();
        filterData.categoryBits = categoryBits;
        m_fixture->SetFilterData(filterData);
    }
}

void Box2DFixtureComponent::setIsSensor(bool isSensor)
{
    m_isSensor = isSensor;
    if (m_fixture != nullptr)
        m_fixture->SetSensor(isSensor);
}

void Box2DFixtureComponent::setDensity(float density)
{
    m_density = density;
    if (m_fixture != nullptr)
        m_fixture->SetDensity(density);
}

void Box2DFixtureComponent::setRestitution(float elasticity)
{
    m_elasticity = elasticity;
    if (m_fixture != nullptr)
        m_fixture->SetRestitution(elasticity);
}

void Box2DFixtureComponent::setFriction(float friction)
{
    m_friction = friction;
    if (m_fixture != nullptr)
        m_fixture->SetFriction(friction);
}

} // flappy
