#include "Box2DRevoluteJointComponent.h"

#include <Entity.h>
#include <Box2D.h>

#include "Box2DBodyManager.h"
#include "Box2DWorldManager.h"

namespace flappy {

Box2DRevoluteJointComponent::Box2DRevoluteJointComponent() {
    subscribe([this](InitEvent) {
        if (isValid())
            init();
    });
}

bool Box2DRevoluteJointComponent::isValid() {
    return (m_bodyA != nullptr) && (m_bodyB != nullptr);
}

void Box2DRevoluteJointComponent::init() {
    auto jointDef = std::make_shared<b2RevoluteJointDef>();
    float sizeFactor = manager<Box2DWorldManager>()->sizeFactor();
    jointDef->bodyA = m_bodyA->body();
    jointDef->bodyB = m_bodyB->body();
    jointDef->localAnchorA.Set(m_localAnchorA.x * sizeFactor,m_localAnchorA.y * sizeFactor);
    jointDef->localAnchorB.Set(m_localAnchorB.x * sizeFactor, m_localAnchorB.y * sizeFactor);
    jointDef->referenceAngle = m_referenceAngle;
    jointDef->lowerAngle = m_lowerAngle;
    jointDef->upperAngle = m_upperAngle;
    jointDef->maxMotorTorque = m_maxMotorTorque;
    jointDef->motorSpeed = m_motorSpeed;
    jointDef->enableLimit = m_enableLimit;
    jointDef->enableMotor = m_enableMotor;
    setJointDef(jointDef);
}

void Box2DRevoluteJointComponent::update() {
    if (isInitialized()) {
        if (isValid())
            init();
        else
            setJointDef(nullptr);
    }
}

}
