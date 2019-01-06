#include "Box2DBodyComponent.h"

#include <TransformComponent.h>
#include <Hierarchy.hpp>

#include "Box2DWorldManager.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

Box2DBodyComponent::Box2DBodyComponent(Handle<Entity> entity)
    : Updatable<Box2DBodyComponent>(entity)
    , m_box2dWorldManager(entity->hierarchy()->manager<Box2DWorldManager>())
{
    m_subscription = m_box2dWorldManager->eventBus().subscribe([this](Box2DWorldManager::ScaleChangedEvent) {
        updatePos();
    });
}

void Box2DBodyComponent::setEntity(Handle<Entity> entity) {
    auto transform = m_transformComponent = entity->component<TransformComponent>();

    b2BodyDef bodyDef;
    bodyDef.linearDamping = m_linearDamping;
    bodyDef.linearVelocity = b2Vec2(m_linearVelocity.x, m_linearVelocity.y);
    bodyDef.type = m_type;
    bodyDef.bullet = m_bullet;
    bodyDef.awake = m_awake;
    bodyDef.angularDamping = m_angularDamping;
    bodyDef.angularVelocity = m_angularVelocity;
    bodyDef.gravityScale = m_gravityScale;
    bodyDef.allowSleep = m_sleepingAllowed;
    bodyDef.fixedRotation = m_fixedRotation;
    
    float sizeFactor = m_box2dWorldManager->sizeFactor();
    
    auto pos = transform->pos();
    bodyDef.position = b2Vec2(pos.x * sizeFactor, pos.y * sizeFactor);
    m_lastTransformPos = transform->pos();
    
    auto angle = transform->angle2DRad();
    bodyDef.angle = angle;
    m_lastTransformAngle = angle;
    
    m_body = m_box2dWorldManager->world().CreateBody(&bodyDef);
    
    auto fixtureComponents = entity->findComponents<Box2DFixtureComponent>(Entity::MaxDepth);
    for (auto fixtureComponent : fixtureComponents)
        fixtureComponent->setBodyComponent(selfHandle());
}
    
Box2DBodyComponent::~Box2DBodyComponent() {
    m_box2dWorldManager->destroyBody(m_body);
}

void Box2DBodyComponent::setMass(float mass) {
    m_mass = mass;
    if (m_body != nullptr)
        setMassInternal(mass);
}

void Box2DBodyComponent::setLinearDamping(float linearDamping) {
    m_linearDamping = linearDamping;
    if (m_body != nullptr)
        m_body->SetLinearDamping(linearDamping);
}

void Box2DBodyComponent::setLinearVelocity(glm::vec2 linearVelocity) {
    m_linearVelocity = linearVelocity;
    if (m_body != nullptr)
        m_body->SetLinearVelocity(b2Vec2(linearVelocity.x, linearVelocity.y));
}

void Box2DBodyComponent::setType(b2BodyType type) {
    m_type = type;
    if (m_body != nullptr)
        m_body->SetType(type);
}

void Box2DBodyComponent::setBullet(bool bullet) {
    m_bullet = bullet;
    if (m_body != nullptr)
        m_body->SetBullet(bullet);
}

void Box2DBodyComponent::setAwake(bool awake) {
    m_awake = awake;
    if (m_body != nullptr)
        m_body->SetAwake(awake);
}

void Box2DBodyComponent::setAngularDamping(float angularDamping) {
    m_angularDamping = angularDamping;
    if (m_body != nullptr)
        m_body->SetAngularDamping(angularDamping);
}

void Box2DBodyComponent::setAngularVelocity(float angularVelocity) {
    m_angularVelocity = angularVelocity;
    if (m_body != nullptr)
        m_body->SetAngularVelocity(angularVelocity);
}

void Box2DBodyComponent::setGravityScale(float gravityScale) {
    m_gravityScale = gravityScale;
    if (m_body != nullptr)
        m_body->SetGravityScale(gravityScale);
}

void Box2DBodyComponent::setSleepingAllowed(bool sleepingAllowed) {
    m_sleepingAllowed = sleepingAllowed;
    if (m_body != nullptr)
        m_body->SetSleepingAllowed(sleepingAllowed);
}

void Box2DBodyComponent::setFixedRotation(bool fixedRotation) {
    m_fixedRotation = fixedRotation;
    if (m_body != nullptr)
        m_body->SetFixedRotation(fixedRotation);
}

void Box2DBodyComponent::destroyFixture(b2Fixture* fixture) {
    if (m_body != nullptr)
        return;
    m_body->DestroyFixture(fixture);
}

b2Fixture *Box2DBodyComponent::createFixture(const b2FixtureDef* def) {
    USER_ASSERT_MSG(m_body != nullptr, "Box2DBodyManager is not initialized. Can't create fixture.");
    return m_body->CreateFixture(def);
}

bool Box2DBodyComponent::testPoint(glm::vec2 point) {
    if (m_body == nullptr)
        return false;
    float sizeFactor = m_box2dWorldManager->sizeFactor();
    b2Vec2 b2Point(point.x * sizeFactor, point.y * sizeFactor);
    b2Fixture* fixture = m_body->GetFixtureList();
    while (fixture) {
        if (fixture->TestPoint(b2Point)) {
            return true;
        }
        fixture = fixture->GetNext();
    }
    return false;
}

void Box2DBodyComponent::applyForce(glm::vec2 force, glm::vec2 point) {
    if (m_body != nullptr)
        m_body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

void Box2DBodyComponent::applyTorque(float torque) {
    if (m_body != nullptr)
        m_body->ApplyTorque(torque, true);
}

void Box2DBodyComponent::applyLinearImulse(glm::vec2 impulse, glm::vec2 point) {
    if (m_body != nullptr)
        m_body->ApplyLinearImpulse(b2Vec2(impulse.x, impulse.y), b2Vec2(point.x, point.y), true);
}

void Box2DBodyComponent::applyAngularImpulse(float impulse) {
    if (m_body != nullptr)
        m_body->ApplyAngularImpulse(impulse, true);
}

void Box2DBodyComponent::updatePos() {
    glm::vec3 newTransformPos = m_transformComponent->pos();
    float newTransformAngle = m_transformComponent->angle2DRad();

    float sizeFactor = m_box2dWorldManager->sizeFactor();

    if ((m_lastTransformAngle == newTransformAngle) && (m_lastTransformPos == newTransformPos)) {
        auto box2DPosition = m_body->GetPosition();
        m_transformComponent->setPos(glm::vec3(box2DPosition.x, box2DPosition.y, m_lastTransformPos.z) / sizeFactor);
        m_transformComponent->setAngle2DRad(m_body->GetAngle());
    } else {
        m_body->SetTransform(b2Vec2(newTransformPos.x * sizeFactor, newTransformPos.y * sizeFactor), newTransformAngle);
    }

    m_lastTransformPos = m_transformComponent->pos();
    m_lastTransformAngle = m_transformComponent->angle2DRad();
}

void Box2DBodyComponent::update(DeltaTime dt) {
    updatePos();
}

void Box2DBodyComponent::setMassInternal(float mass) {
    b2MassData massData;
    m_body->GetMassData(&massData);
    massData.mass = mass;
    m_body->SetMassData(&massData);
}

} // flappy
