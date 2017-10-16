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
    entity()->events()->post(MeshChangedEvent());
}

void MeshComponent::setColor(Color color) {
    m_color = color;
    entity()->events()->post(MeshChangedEvent());
}

} // flappy
