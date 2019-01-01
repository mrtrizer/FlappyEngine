#include "Box2DWorldManager.h"
#include "Box2DRayCastCallback.h"
#include "Box2DFixtureComponent.h"

#include <Box2D.h>

namespace flappy {

Box2DWorldManager::Box2DWorldManager(Handle<Hierarchy> hierarchy)
    : Updatable<Box2DWorldManager>(hierarchy)
    , m_world(std::make_unique<b2World>(b2Vec2(0.0f, -9.8f)))
{
    m_world->SetContactListener(&m_contactListener);
    m_contactListener.setContactCallback([hierarchy](b2Contact* contact, Box2DContactListener::ContactPhase constactPhase) {
        auto objectIdA = reinterpret_cast<ObjectId>(contact->GetFixtureA()->GetUserData());
        auto objectIdB = reinterpret_cast<ObjectId>(contact->GetFixtureB()->GetUserData());
        Handle<Box2DFixtureComponent> fixtureComponentA = hierarchy->memoryManager().findObject(objectIdA);
        Handle<Box2DFixtureComponent> fixtureComponentB = hierarchy->memoryManager().findObject(objectIdB);
        if (fixtureComponentA.isValid())
            fixtureComponentA->handleContact(contact, constactPhase, objectIdB);
        if (fixtureComponentB.isValid())
            fixtureComponentB->handleContact(contact, constactPhase, objectIdA);
    });
}

b2Joint* Box2DWorldManager::createJoint(std::shared_ptr<b2JointDef> jointDef) {
    auto joint = m_world->CreateJoint(jointDef.get());
    return joint;
}

void Box2DWorldManager::destroyJoint(b2Joint* jointPtr) {
    m_world->DestroyJoint(jointPtr);
}

void Box2DWorldManager::destroyBody(b2Body* bodyPtr) {
    m_bodiesToDestroy.emplace_back(bodyPtr);
}

void Box2DWorldManager::setGravity(glm::vec2 gravity) {
    m_world->SetGravity(b2Vec2(gravity.x, gravity.y));
}

float Box2DWorldManager::sizeFactor() const
{
    return m_sizeFactor;
}

void Box2DWorldManager::setSizeFactor(float sizeFactor)
{
    m_sizeFactor = sizeFactor;
    m_eventBus.post(ScaleChangedEvent{});
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
    m_world->RayCast(&rayCastCallback, point1, point2);
    auto fixture = rayCastCallback.fixture();
    RayCastData rayCastData;
    rayCastData.fixtures.push_back(*reinterpret_cast<Handle<Box2DFixtureComponent>*>(fixture->GetUserData()));
    return rayCastData;
}

void Box2DWorldManager::update(DeltaTime dt) {
    m_world->Step((float32)dt, (int32)m_velocityIterations, (int32)m_positionIterations);
    destroyWaitingObjects();
}

void Box2DWorldManager::destroyWaitingObjects() {
    for (auto bodyPtr : m_bodiesToDestroy)
        m_world->DestroyBody(bodyPtr);
    m_bodiesToDestroy.clear();
}

} // flappy
