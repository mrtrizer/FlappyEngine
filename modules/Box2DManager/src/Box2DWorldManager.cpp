#include "Box2DWorldManager.h"
#include "Box2DRayCastCallback.h"
#include "Box2DFixtureComponent.h"

#include <Box2D.h>

#include <Entity.h>

namespace flappy {

Box2DWorldManager::Box2DWorldManager():m_world(b2Vec2(0.0f, -9.8f))
{
    m_world.SetContactListener(&m_contactListener);
    m_contactListener.setContactStartCallback([this](b2Contact* contact) {
        sendContactEvent<ContactStartEvent>(contact);
    });
    m_contactListener.setContactEndCallback([this](b2Contact* contact) {
        sendContactEvent<ContactEndEvent>(contact);
    });

    events()->subscribe([this](UpdateEvent e) {
        update(e.dt);
    });
}

template<typename ContactEventT>
EventHandle Box2DWorldManager::createContactEvent(b2Contact* contact, SafePtr<Box2DFixtureComponent> otherFixture) {
    ContactEventT contactEvent;
    contactEvent.fixture = otherFixture;
    auto manifold = contact->GetManifold();
    contactEvent.pos = {manifold->localPoint.x, manifold->localPoint.y};
    return EventHandle(contactEvent);
}

template<typename ContactEventT>
void Box2DWorldManager::sendContactEvent(b2Contact* contact) {
    auto fixtureA = contact->GetFixtureA();
    auto fixtureComponentA = reinterpret_cast<Box2DFixtureComponent*>(fixtureA->GetUserData())->selfPointer();

    auto fixtureB = contact->GetFixtureB();
    auto fixtureComponentB = reinterpret_cast<Box2DFixtureComponent*>(fixtureB->GetUserData())->selfPointer();

    { // Contact event A
        auto evenHandle = createContactEvent<ContactEventT>(contact, fixtureComponentB);
        auto entityA = fixtureComponentA->entityRef();
        m_contactEventHolders.push_back({entityA, std::move(evenHandle)});
    }
    { // ContactEvent B
        auto evenHandle = createContactEvent<ContactEventT>(contact, fixtureComponentA);
        auto entityB = fixtureComponentB->entityRef();
        m_contactEventHolders.push_back({entityB, std::move(evenHandle)});
    }
}

b2Joint* Box2DWorldManager::createJoint(std::shared_ptr<b2JointDef> jointDef) {
    auto joint = m_world.CreateJoint(jointDef.get());
    return joint;
}

void Box2DWorldManager::destroyJoint(b2Joint* joint) {
    m_world.DestroyJoint(joint);
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
    entity()->events()->post(Box2DWorldScaleChanged());
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

Box2DWorldManager::RayCastData Box2DWorldManager::rayCast(glm::vec2 start, glm::vec2 end) {
    Box2DRayCastCallback rayCastCallback;
    b2Vec2 point1(start.x, start.y);
    b2Vec2 point2(end.x, end.y);
    m_world.RayCast(&rayCastCallback, point1, point2);
    auto fixture = rayCastCallback.fixture();
    RayCastData rayCastData;
    rayCastData.fixtures.push_back(reinterpret_cast<Box2DFixtureComponent*>(fixture->GetUserData()));
    return rayCastData;
}

void Box2DWorldManager::sendContactEvents() {
    for (const auto& contactEventHolder : m_contactEventHolders) {
        if (contactEventHolder.entity != nullptr)
            contactEventHolder.entity->events()->post(contactEventHolder.eventHandle);
    }
    m_contactEventHolders.clear();
}

void Box2DWorldManager::update(DeltaTime dt) {
    m_world.Step((float32)dt, (int32)m_velocityIterations, (int32)m_positionIterations);
    sendContactEvents();
}


} // flappy
