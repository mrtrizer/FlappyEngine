#pragma once

#include <glm/vec2.hpp>

#include <Handle.hpp>

#include "Box2DFixtureComponent.h"

namespace flappy {
    
class Hierarchy;
class Box2DWorldManager;

class Box2DBoxComponent: public Box2DFixtureComponent {
public:
    Box2DBoxComponent(Handle<Entity> entity);

    void setSize(glm::vec2 size);
    glm::vec2 size() { return m_size; }

private:
    Handle<Box2DWorldManager> m_box2DWorldManager;
    glm::vec2 m_size;

    void init(glm::vec2 size);
};

} // flappy
