#include "Box2DRevoluteJointComponent.h"

#include <Entity.h>
#include <Box2DBodyComponent.h>
#include <Box2D.h>

namespace flappy {

Box2DRevoluteJointComponent::Box2DRevoluteJointComponent() {

    subscribe([this](InitEvent) {
        auto jointDef = std::make_shared<b2RevoluteJointDef>();
        jointDef->collideConnected = false;
        jointDef->localAnchorA.Set(2,2);//the top right corner of the box
        jointDef->localAnchorB.Set(0,0);
        jointDef->enableMotor = true;
        jointDef->motorSpeed = 2.0f;
        setJointDef(jointDef);
    });

}

}
