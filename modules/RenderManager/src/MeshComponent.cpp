#include "MeshComponent.h"

#include "MaterialRes.h"
#include "RenderElementFactory.h"

namespace flappy {

MeshComponent::MeshComponent(Handle<Hierarchy> hierarchy)
    : m_renderElementFactory(hierarchy->manager<RenderElementFactory>())
    , m_materialResManager(hierarchy->manager<ResManager<MaterialRes>>())
    , m_materialRes(nullptr)
{
    m_materialRes = m_materialResManager->getRes("default_material", ExecType::SYNC);
    
}

void MeshComponent::setEntity(Handle<Entity> entity) {
    m_entity = entity;
    m_renderElement = m_renderElementFactory->createMeshRender(entity);
}
    
MeshComponent::~MeshComponent() {
    m_entity->removeComponent(m_renderElement);
}

void MeshComponent::update(DeltaTime dt) {
    if (m_materialRes->resUpdated()) {
        m_materialRes = m_materialRes->lastRes();
    }
}

void MeshComponent::setVertices(const std::vector<glm::vec3>& vertices) {
    m_vertices = vertices;
    m_eventBus.post(MeshChangedEvent());
}

void MeshComponent::setUVs(std::vector<glm::vec2> uvs) {
    m_uvs = uvs;
    m_eventBus.post(MeshChangedEvent());
}

void MeshComponent::setMaterialRes(std::shared_ptr<MaterialRes> materialRes) {
    m_materialRes = materialRes;
    m_eventBus.post(MeshChangedEvent());
}

} // flappy
