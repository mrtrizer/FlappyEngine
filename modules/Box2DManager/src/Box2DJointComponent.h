#pragma once

#include <memory>

#include <Handle.hpp>

class b2Joint;
struct b2JointDef;

namespace flappy {

class Hierarchy;
class Box2DWorldManager;

class [[component]] Box2DJointComponent {
public:
    Box2DJointComponent(Handle<Hierarchy> hierarchy);
    ~Box2DJointComponent();

protected:
    void setJointDef(const std::shared_ptr<b2JointDef>& jointDef);
    const std::shared_ptr<b2JointDef>& jointDef() const { return m_jointDef; }

private:
    b2Joint* m_joint = nullptr;
    std::shared_ptr<b2JointDef> m_jointDef;
    Handle<Box2DWorldManager> m_worldManager;
};

} // flappy
