#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <Updatable.hpp>
#include <EnableSelfHandle.hpp>
#include <Box2D/Box2D.h>
#include <Handle.hpp>
#include <ISubscription.h>

namespace flappy {
    
class Hierarchy;
class Box2DWorldManager;
class TransformComponent;

class REFLECT Box2DBodyComponent
    : public Updatable<Box2DBodyComponent>
    , public EnableSelfHandle<Box2DBodyComponent> {
public:
    Box2DBodyComponent(Handle<Entity> entity);
    void addedToEntity(Handle<Entity> entity);
    ~Box2DBodyComponent();

    void update(DeltaTime dt);
    
    b2Body* body() { return m_body; }

    PROP void setMass(float mass);
    PROP float mass() { return m_mass; }

    PROP void setLinearDamping(float linearDamping);
    PROP float linearDamping() { return m_linearDamping; }

    PROP void setLinearVelocity(glm::vec2 linearVelocity);
    PROP glm::vec2 linearVelocity() { return m_linearVelocity; }

    PROP void setType(b2BodyType type);
    PROP b2BodyType type() { return m_type; }

    PROP void setBullet(bool bullet);
    PROP bool bullet() { return m_bullet; }

    PROP void setAwake(bool awake);
    PROP bool awake() { return m_awake; }

    PROP void setAngularDamping(float angularDamping);
    PROP float angularDamping() { return m_angularDamping; }

    PROP void setAngularVelocity(float angularVelocity);
    PROP float angularVelocity() { return m_angularVelocity; }

    PROP void setGravityScale(float gravityScale);
    PROP float gravityScale() { return m_gravityScale; }

    PROP void setSleepingAllowed(bool sleepingAllowed);
    PROP bool sleepingAllowed() { return m_sleepingAllowed; }

    PROP void setFixedRotation(bool fixedRotation);
    PROP bool fixedRotation() { return m_fixedRotation; }

    void destroyFixture(b2Fixture *fixture);
    b2Fixture* createFixture(const b2FixtureDef* def);

    bool testPoint(glm::vec2 point);

    FUNC void applyForce(glm::vec2 force, glm::vec2 point);
    FUNC void applyTorque(float torque);
    FUNC void applyLinearImulse(glm::vec2 impulse, glm::vec2 point);
    FUNC void applyAngularImpulse(float impulse);

private:
    Handle<Box2DWorldManager> m_box2dWorldManager;
    Handle<TransformComponent> m_transformComponent;
    std::shared_ptr<ISubscription> m_subscription;
    b2Body* m_body = nullptr;
    glm::vec3 m_lastTransformPos;
    float m_lastTransformAngle = 0.0f;

    float m_mass = 0.0f;
    float m_linearDamping = 0.0f;
    glm::vec2 m_linearVelocity;
    b2BodyType m_type = b2_dynamicBody;
    bool m_bullet = false;
    bool m_awake = true;
    bool m_angularDamping = 0.0f;
    bool m_angularVelocity = 0.0f;
    bool m_gravityScale = 1.0f;
    bool m_sleepingAllowed = false;
    bool m_fixedRotation = false;

    void updatePos();
    void setMassInternal(float mass);
};

} // flappy
