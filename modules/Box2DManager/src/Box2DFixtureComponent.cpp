#include "Box2DFixtureComponent.h"

#include "Box2DBodyManager.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

void Box2DFixtureComponent::setEntity(Handle<Entity> entity) {
    while (entity != nullptr) {
        auto body = entity->findComponent<Box2DBodyManager>();
        if (body != nullptr) {
            m_box2dBodyComponent = body;
            break;
        }
        entity = entity->parent();
    }
}
    
void Box2DFixtureComponent::setBodyComponent(Handle<Box2DBodyManager> body) {
    m_box2dBodyComponent = body;
    initFixture(body, m_shape);
}

void Box2DFixtureComponent::resetBodyComponent(Handle<Box2DBodyManager> body) {
    m_box2dBodyComponent = nullptr;
    deinitFixture(body);
}

Box2DFixtureComponent::~Box2DFixtureComponent() {
    deinitFixture(m_box2dBodyComponent);
    m_fixture = nullptr;
}
    
template<typename ContactEventT>
EventHandle createContactEvent(b2Contact* contact, Handle<Box2DFixtureComponent> otherFixture) {
    ContactEventT contactEvent;
    contactEvent.fixture = otherFixture;
    auto manifold = contact->GetManifold();
    contactEvent.pos = {manifold->localPoint.x, manifold->localPoint.y};
    return EventHandle(contactEvent);
}
    
void Box2DFixtureComponent::handleContact(b2Contact* contact,
                                          Box2DContactListener::ContactPhase contactPhase,
                                          Handle<Box2DFixtureComponent> other)
{
    switch (contactPhase) {
        case Box2DContactListener::ContactPhase::BEGIN:
            m_eventBus.post(createContactEvent<ContactStartEvent>(contact, other));
            break;
        case Box2DContactListener::ContactPhase::END:
            m_eventBus.post(createContactEvent<ContactEndEvent>(contact, other));
            break;
    }
    
}
    
void Box2DFixtureComponent::deinitFixture(Handle<Box2DBodyManager> body) {
    if (body != nullptr && m_fixture != nullptr) {
        body->destroyFixture(m_fixture);
    }
}

void Box2DFixtureComponent::initFixture(Handle<Box2DBodyManager> body, std::shared_ptr<b2Shape> shape) {
    if (body == nullptr)
        return;
    
    deinitFixture(body);
    
    if (shape != nullptr) {
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
        m_selfHandle = selfHandle();
        fixtureDef.userData = (void*)&m_selfHandle;

        m_fixture = body->createFixture(&fixtureDef);
    }
}

void Box2DFixtureComponent::setShape(std::shared_ptr<b2Shape> shape) {
    m_shape = shape;
    initFixture(m_box2dBodyComponent, shape);
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
