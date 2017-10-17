#pragma once

#include <Component.h>

#include <glm/vec3.hpp>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DBodyComponent: public Component<Box2DBodyComponent> {
public:
    Box2DBodyComponent();

    b2Body* body() { return m_body; }

    void setMass(float mass);

    void setLinearDamping(float linearDamping);
    float linearDamping() { return m_linearDamping; }

    void setLinearVelocity(glm::vec2 linearVelocity);
    glm::vec2 linearVelocity() { return m_linearVelocity; }

    void setType(b2BodyType type);
    b2BodyType type() { return m_type; }

    void setBullet(bool bullet);
    bool bullet() { return m_bullet; }

    void setAwake(bool awake);
    bool awake() { return m_awake; }

    void setAngularDamping(float angularDamping);
    float angularDamping() { return m_angularDamping; }

    void setAngularVelocity(float angularVelocity);
    float angularVelocity() { return m_angularVelocity; }

    void setGravityScale(float gravityScale);
    float gravityScale() { return m_gravityScale; }

    void setSleepingAllowed(bool sleepingAllowed);
    bool sleepingAllowed() { return m_sleepingAllowed; }

    void setFixedRotation(bool fixedRotation);
    bool fixedRotation() { return m_fixedRotation; }

    void destroyFixture(b2Fixture *fixture);
    b2Fixture* createFixture(const b2FixtureDef* def);

    bool testPoint(glm::vec2 point);



private:
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
    void update(DeltaTime dt);
    void setMassInternal(float mass);
};

} // flappy
