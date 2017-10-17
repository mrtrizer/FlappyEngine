#pragma once

#include "Box2DJointComponent.h"

namespace flappy {

class Box2DRevoluteJointComponent: public Box2DJointComponent {
public:
    Box2DRevoluteJointComponent();

    void setTargetBody(SafePtr<Box2DBodyComponent>);
    SafePtr<Box2DBodyComponent> targetBody() { return m_targetBody; }

private:
    SafePtr<Box2DBodyComponent> m_targetBody;

    void init();
};

} // flappy
