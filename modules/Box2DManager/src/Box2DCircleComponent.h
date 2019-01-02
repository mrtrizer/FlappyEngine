#pragma once

#include <Handle.hpp>

#include "Box2DFixtureComponent.h"

namespace flappy {
    
class Hierarchy;
class Box2DWorldManager;
    
class Box2DCircleComponent: public Box2DFixtureComponent {
public:
    Box2DCircleComponent(Handle<Entity> entity);

    void setRadius(float radius);
    float radius() { return m_radius; }

private:
    float m_radius;
    Handle<Box2DWorldManager> m_worldManager;
};

} // flappy
