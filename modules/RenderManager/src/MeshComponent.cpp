#include "MeshComponent.h"

namespace flappy {

MeshComponent::MeshComponent()
{
    addDependency(RenderElementFactory::id());

    subscribe([this](InitEvent) {
        m_renderElement = manager<RenderElementFactory>()->createMeshRender(selfPointer());
        entity()->addComponent(m_renderElement);
    });

    subscribe([this](DeinitEvent) {
        entity()->removeComponent(m_renderElement);
        m_renderElement.reset();
    });
}

void MeshComponent::setVertices(std::vector<glm::vec3> vertices) {
    m_vertices = vertices;
    if (isInitialized()) {
        m_renderElement->setActive(false);
        m_renderElement->setActive(true);
    }
}

} // flappy
