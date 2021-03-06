#include "Render.h"

#include <RenderManager.h>
#include <Entity.hpp>

namespace flappy {

Render::Render(Handle<Entity> entity)
    : m_renderManager(entity->hierarchy()->manager<RenderManager>())
    , m_entity(entity)
{
    m_renderManager->registerRenderElement(selfHandle());
}

Render::~Render() {
    m_renderManager->unregisterRenderElement(selfHandle());
}

/// Calls update method if need (if externUpdate() was invoked before)
/// and than calls virtual draw() method, reimplemented in concrete childs.
void Render::redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) {
    draw(pMartrix, mvMatrix);
}

} // flappy
