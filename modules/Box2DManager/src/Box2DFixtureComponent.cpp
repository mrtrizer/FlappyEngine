#include "Box2DFixtureComponent.h"

#include <Entity.h>

#include "Box2DBodyComponent.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

Box2DFixtureComponent::Box2DFixtureComponent() : Component({}, {Box2DBodyComponent::id()}) {

}

void Box2DFixtureComponent::init() {
    auto bodyComponent = entity()->component<Box2DBodyComponent>();
    b2FixtureDef fixtureDef;

    fixtureDef.shape = m_shape.get();

    // physics params
    fixtureDef.friction = m_friction;
    fixtureDef.restitution = m_elasticity;
    fixtureDef.density = m_density;
    fixtureDef.isSensor = m_isSensor;

    // filter
    fixtureDef.filter.categoryBits = m_categoryBits;
    fixtureDef.filter.maskBits = m_maskBits;
    fixtureDef.filter.groupIndex = m_groupIndex;

    m_fixture = bodyComponent->body().CreateFixture(&fixtureDef);
}

void Box2DFixtureComponent::deinit() {
    auto bodyComponent = entity()->component<Box2DBodyComponent>();
    bodyComponent->body().DestroyFixture(m_fixture);
    m_fixture = nullptr;
}

b2Fixture* Box2DFixtureComponent::fixture() const
{
    return m_fixture;
}

void Box2DFixtureComponent::setShape(std::unique_ptr<b2Shape>&& shape) {
    m_shape = std::move(shape);
    if (m_fixture != nullptr) {
       LOGE("Setting of shape after initialization is not supported");
    }
}

int16_t Box2DFixtureComponent::groupIndex() const
{
    return m_groupIndex;
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

uint16_t Box2DFixtureComponent::maskBits() const
{
    return m_maskBits;
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

uint16_t Box2DFixtureComponent::categoryBits() const
{
    return m_categoryBits;
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

bool Box2DFixtureComponent::isSensor() const
{
    return m_isSensor;
}

void Box2DFixtureComponent::setIsSensor(bool isSensor)
{
    m_isSensor = isSensor;
    if (m_fixture != nullptr)
        m_fixture->SetSensor(isSensor);
}

float Box2DFixtureComponent::density() const
{
    return m_density;
}

void Box2DFixtureComponent::setDensity(float density)
{
    m_density = density;
    if (m_fixture != nullptr)
        m_fixture->SetDensity(density);
}

float Box2DFixtureComponent::elasticity() const
{
    return m_elasticity;
}

void Box2DFixtureComponent::setElasticity(float elasticity)
{
    m_elasticity = elasticity;
    if (m_fixture != nullptr)
        m_fixture->SetRestitution(elasticity);
}

float Box2DFixtureComponent::friction() const
{
    return m_friction;
}

void Box2DFixtureComponent::setFriction(float friction)
{
    m_friction = friction;
    if (m_fixture != nullptr)
        m_fixture->SetFriction(friction);
}

} // flappy
