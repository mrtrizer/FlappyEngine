#include "Box2DJointComponent.h"

#include <Box2D.h>

#include "Box2DWorldManager.h"

namespace flappy {

Box2DJointComponent::Box2DJointComponent(Handle<Hierarchy> hierarchy)
    : m_worldManager(hierarchy->manager<Box2DWorldManager>())
{}
    
Box2DJointComponent::~Box2DJointComponent() {
    if (m_joint != nullptr) {
        m_worldManager->destroyJoint(m_joint);
        m_joint = nullptr;
    }
}

void Box2DJointComponent::setJointDef(const std::shared_ptr<b2JointDef>& jointDef) {
    m_jointDef = jointDef;
    if (m_joint != nullptr)
        m_worldManager->destroyJoint(m_joint);
    m_joint = nullptr;
    if (jointDef != nullptr)
        m_joint = m_worldManager->createJoint(jointDef);
}

} // flappy
