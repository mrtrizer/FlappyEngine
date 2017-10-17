#include "Box2DRevoluteJointComponent.h"

#include <Entity.h>
#include <Box2DBodyComponent.h>
#include <Box2D.h>

namespace flappy {

Box2DRevoluteJointComponent::Box2DRevoluteJointComponent() {

    subscribe([this](InitEvent) {
        if (m_targetBody != nullptr)
            init();
    });

}

void Box2DRevoluteJointComponent::setTargetBody(SafePtr<Box2DBodyComponent> targetBody) {
    m_targetBody = targetBody;

    if (isInitialized() && (m_targetBody != nullptr)) {
        init();
    }
}

void Box2DRevoluteJointComponent::init() {
    auto jointDef = std::make_shared<b2RevoluteJointDef>();
    jointDef->bodyA = entity()->component<Box2DBodyComponent>()->body();
    jointDef->bodyB = m_targetBody->body();
    jointDef->collideConnected = false;
    jointDef->localAnchorA.Set(2,2);//the top right corner of the box
    jointDef->localAnchorB.Set(0,0);
    jointDef->enableMotor = true;
    jointDef->motorSpeed = 10.0f;
    jointDef->maxMotorTorque = 1000.0f;
    setJointDef(jointDef);
}

}
