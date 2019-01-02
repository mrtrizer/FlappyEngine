#pragma once

#include <glm/vec2.hpp>

#include <Handle.hpp>

#include "Box2DJointComponent.h"

namespace flappy {

class Entity;
class Box2DBodyManager;

class [[component]] Box2DRevoluteJointComponent: public Box2DJointComponent {
public:
    Box2DRevoluteJointComponent(Handle<Entity> entity);
    
    void setBodyA(Handle<Box2DBodyManager> bodyA) { m_bodyA = bodyA; update(); }
    Handle<Box2DBodyManager> bodyA() { return m_bodyA; }

    void setBodyB(Handle<Box2DBodyManager> bodyB) { m_bodyB = bodyB; update(); }
    Handle<Box2DBodyManager> bodyB() { return m_bodyB; }

    glm::vec2 localAnchorA() { return m_localAnchorA; }
    void setLocalAnchorA(const glm::vec2& localAnchorA) { m_localAnchorA = localAnchorA; update(); }

    glm::vec2 localAnchorB() { return m_localAnchorB; }
    void setLocalAnchorB(const glm::vec2& localAnchorB) { m_localAnchorB = localAnchorB; update(); }

    float referenceAngle() { return m_referenceAngle; }
    void setRefenreceAngle(float referenceAngle) { m_referenceAngle = referenceAngle; update();}

    float lowerAngle() { return m_lowerAngle; }
    void setLowerAngle(float lowerAngle) { m_lowerAngle = lowerAngle; update();}

    float upperAngle() { return m_upperAngle; }
    void setUpperAngle(float upperAngle) { m_upperAngle = upperAngle; update();}

    float maxMotorTorque() { return m_maxMotorTorque; }
    void setMaxMotorTorque(float maxMotorTorque) { m_maxMotorTorque = maxMotorTorque; update();}

    float motorSpeed() { return m_motorSpeed; }
    void setMotorSpeed(float motorSpeed) { m_motorSpeed = motorSpeed; update();}

    bool enableLimit() { return m_enableLimit; }
    void setEnableLimit(bool enableLimit) { m_enableLimit = enableLimit; update();}

    bool enableMotor() { return m_enableMotor; }
    void setEnableMotor(bool enableMotor) { m_enableMotor = enableMotor; update();}

private:
    Handle<Box2DWorldManager> m_worldManager;
    Handle<Box2DBodyManager> m_bodyA;
    Handle<Box2DBodyManager> m_bodyB;
    glm::vec2 m_localAnchorA;
    glm::vec2 m_localAnchorB;
    float m_referenceAngle = 0.0f;
    float m_lowerAngle = 0.0f;
    float m_upperAngle = 0.0f;
    float m_maxMotorTorque = 0.0f;
    float m_motorSpeed = 0.0f;
    bool m_enableLimit = false;
    bool m_enableMotor = false;

    bool isValid();
    void init();
    void update();
};

} // flappy
