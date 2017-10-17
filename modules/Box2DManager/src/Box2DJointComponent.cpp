#include "Box2DJointComponent.h"

#include <Box2D.h>

#include <Entity.h>

#include "Box2DBodyComponent.h"
#include "Box2DWorldManager.h"

namespace flappy {

Box2DJointComponent::Box2DJointComponent() {

    addDependency(Box2DBodyComponent::id());
    addDependency(Box2DWorldManager::id());

    subscribe([this](InitEvent) {
        if (m_jointDef != nullptr)
            m_joint = manager<Box2DWorldManager>()->createJoint(m_jointDef);
    });

    subscribe([this](DeinitEvent) {
        if (m_joint != nullptr) {
            manager<Box2DWorldManager>()->destroyJoint(m_joint);
            m_joint = nullptr;
        }
    });
}

void Box2DJointComponent::setJointDef(std::shared_ptr<b2JointDef> jointDef) {
    m_jointDef = jointDef;
    if (isInitialized()) {
        if (m_joint != nullptr)
            manager<Box2DWorldManager>()->destroyJoint(m_joint);
        m_joint = manager<Box2DWorldManager>()->createJoint(jointDef);
    }
}

} // flappy
