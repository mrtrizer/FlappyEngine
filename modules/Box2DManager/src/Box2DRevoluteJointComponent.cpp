#include "Box2DRevoluteJointComponent.h"

#include <Box2D.h>

#include "Box2DBodyComponent.h"
#include "Box2DWorldManager.h"

namespace flappy {

Box2DRevoluteJointComponent::Box2DRevoluteJointComponent(Handle<Entity> entity)
    : Box2DJointComponent(entity->hierarchy())
    , m_worldManager(entity->hierarchy()->manager<Box2DWorldManager>())
{}
    
bool Box2DRevoluteJointComponent::isValid() {
    return (m_bodyA != nullptr) && (m_bodyB != nullptr);
}

void Box2DRevoluteJointComponent::init() {
    auto jointDef = std::make_shared<b2RevoluteJointDef>();
    float sizeFactor = m_worldManager->sizeFactor();
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
    if (isValid())
        init();
    else
        setJointDef(nullptr);
}

}
