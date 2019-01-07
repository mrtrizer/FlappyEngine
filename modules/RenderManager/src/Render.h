#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include <Handle.hpp>
#include <EnableSelfHandle.hpp>

namespace flappy {
    
class Entity;
class Hierarchy;
class RenderManager;

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class REFLECT Render : public EnableSelfHandle<Render> {
public:
    Render(Handle<Entity> entity);
    virtual ~Render();
    void redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix);
    Handle<Entity> entity() { return m_entity; }
protected:
    virtual void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) = 0;
private:
    Handle<RenderManager> m_renderManager;
    Handle<Entity> m_entity;
};

} // flappy
