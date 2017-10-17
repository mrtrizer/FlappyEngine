#pragma once

#include <Component.h>

struct b2Joint;
struct b2JointDef;

namespace flappy {

class Box2DBodyComponent;

class Box2DJointComponent: public Component<Box2DJointComponent> {
public:
    Box2DJointComponent();

protected:
    void setJointDef(std::shared_ptr<b2JointDef> jointDef);
    std::shared_ptr<b2JointDef> jointDef() { return m_jointDef; }

private:

    b2Joint* m_joint = nullptr;
    std::shared_ptr<b2JointDef> m_jointDef;
};

} // flappy
