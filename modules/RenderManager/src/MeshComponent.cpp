#include "MeshComponent.h"

#include "MaterialRes.h"

namespace flappy {

MeshComponent::MeshComponent()
    : m_materialRes(std::make_shared<MaterialRes>(nullptr))
{
    m_materialRes->setVec4("uColor", {1.0f, 1.0f, 1.0f, 1.0f});

    addDependency(RenderElementFactory::id());

    subscribe([this](InitEvent) {
        m_renderElement = manager<RenderElementFactory>()->createMeshRender(selfPointer());
        entity()->addComponent(m_renderElement);
    });

    subscribe([this](DeinitEvent) {
        entity()->removeComponent(m_renderElement);
        m_renderElement.reset();
    });

    subscribe([this](UpdateEvent) {
        if (m_materialRes->nextRes() != m_materialRes) {
            m_materialRes = m_materialRes->lastRes();
        }
    });
}

void MeshComponent::setVertices(std::vector<glm::vec3> vertices) {
    m_vertices = vertices;
    entity()->events()->post(MeshChangedEvent());
}

void MeshComponent::setUVs(std::vector<glm::vec2> uvs) {
    m_uvs = uvs;
    entity()->events()->post(MeshChangedEvent());
}

void MeshComponent::setMaterialRes(std::shared_ptr<MaterialRes> materialRes) {
    m_materialRes = materialRes;
    entity()->events()->post(MeshChangedEvent());
}

} // flappy
