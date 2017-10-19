#include "Box2DBodyManager.h"

#include <TransformComponent.h>
#include <Entity.h>

#include "Box2DWorldManager.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

Box2DBodyManager::Box2DBodyManager() {
    addDependency(Box2DWorldManager::id());
    addDependency(TransformComponent::id());

    events()->subscribe([this](InitEvent) {
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

        float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();

        auto pos = entity()->component<TransformComponent>()->pos();
        bodyDef.position = b2Vec2(pos.x * sizeFactor, pos.y * sizeFactor);
        m_lastTransformPos = entity()->component<TransformComponent>()->pos();

        auto angle = entity()->component<TransformComponent>()->angle2DRad();
        bodyDef.angle = angle;
        m_lastTransformAngle = angle;

        m_body = manager<Box2DWorldManager>()->world().CreateBody(&bodyDef);
    });

    events()->subscribe([this](Box2DWorldManager::Box2DWorldScaleChanged) {
        if (isInitialized())
            updatePos();
    });

    events()->subscribe([this](DeinitEvent) {
        manager<Box2DWorldManager>()->world().DestroyBody(m_body);
    });

    events()->subscribe([this](UpdateEvent e) {
        update(e.dt);
    });
}

void Box2DBodyManager::setMass(float mass) {
    m_mass = mass;
    if (isInitialized())
        setMassInternal(mass);
}

void Box2DBodyManager::setLinearDamping(float linearDamping) {
    m_linearDamping = linearDamping;
    if (isInitialized())
        m_body->SetLinearDamping(linearDamping);
}

void Box2DBodyManager::setLinearVelocity(glm::vec2 linearVelocity) {
    m_linearVelocity = linearVelocity;
    if (isInitialized())
        m_body->SetLinearVelocity(b2Vec2(linearVelocity.x, linearVelocity.y));
}

void Box2DBodyManager::setType(b2BodyType type) {
    m_type = type;
    if (isInitialized())
        m_body->SetType(type);
}

void Box2DBodyManager::setBullet(bool bullet) {
    m_bullet = bullet;
    if (isInitialized())
        m_body->SetBullet(bullet);
}

void Box2DBodyManager::setAwake(bool awake) {
    m_awake = awake;
    if (isInitialized())
        m_body->SetAwake(awake);
}

void Box2DBodyManager::setAngularDamping(float angularDamping) {
    m_angularDamping = angularDamping;
    if (isInitialized())
        m_body->SetAngularDamping(angularDamping);
}

void Box2DBodyManager::setAngularVelocity(float angularVelocity) {
    m_angularVelocity = angularVelocity;
    if (isInitialized())
        m_body->SetAngularVelocity(angularVelocity);
}

void Box2DBodyManager::setGravityScale(float gravityScale) {
    m_gravityScale = gravityScale;
    if (isInitialized())
        m_body->SetGravityScale(gravityScale);
}

void Box2DBodyManager::setSleepingAllowed(bool sleepingAllowed) {
    m_sleepingAllowed = sleepingAllowed;
    if (isInitialized())
        m_body->SetSleepingAllowed(sleepingAllowed);
}

void Box2DBodyManager::setFixedRotation(bool fixedRotation) {
    m_fixedRotation = fixedRotation;
    if (isInitialized())
        m_body->SetFixedRotation(fixedRotation);
}

void Box2DBodyManager::destroyFixture(b2Fixture* fixture) {
    if (!isInitialized())
        return;
    m_body->DestroyFixture(fixture);
}

b2Fixture *Box2DBodyManager::createFixture(const b2FixtureDef* def) {
    if (!isInitialized())
        throw std::runtime_error("Box2DBodyManager is not initialized. Can't create fixture.");
    return m_body->CreateFixture(def);
}

bool Box2DBodyManager::testPoint(glm::vec2 point) {
    if (!isInitialized())
        return false;
    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();
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

void Box2DBodyManager::updatePos() {
    glm::vec3 newTransformPos = entity()->component<TransformComponent>()->pos();
    float newTransformAngle = entity()->component<TransformComponent>()->angle2DRad();

    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();

    if ((m_lastTransformAngle == newTransformAngle) && (m_lastTransformPos == newTransformPos)) {
        auto box2DPosition = m_body->GetPosition();
        entity()->component<TransformComponent>()->setPos(glm::vec3(box2DPosition.x, box2DPosition.y, m_lastTransformPos.z) / sizeFactor);
        entity()->component<TransformComponent>()->setAngle2DRad(m_body->GetAngle());
    } else {
        m_body->SetTransform(b2Vec2(newTransformPos.x * sizeFactor, newTransformPos.y * sizeFactor), newTransformAngle);
    }

    m_lastTransformPos = entity()->component<TransformComponent>()->pos();
    m_lastTransformAngle = entity()->component<TransformComponent>()->angle2DRad();
}

void Box2DBodyManager::update(DeltaTime dt) {
    updatePos();
}

void Box2DBodyManager::setMassInternal(float mass) {
    b2MassData massData;
    m_body->GetMassData(&massData);
    massData.mass = mass;
    m_body->SetMassData(&massData);
}

} // flappy
