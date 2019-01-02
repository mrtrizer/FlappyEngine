#pragma once

#include <IEvent.h>
#include <Render.h>
#include <EventBus.h>

#include "MaterialRes.h"
#include "RenderElementFactory.h"

namespace flappy {

class MaterialRes;
class RenderElementFactory;
template <typename T>
class ResManager;

class [[component]] MeshComponent : public EnableSelfHandle<MeshComponent> {
public:
    struct MeshChangedEvent : public IEvent {};

    MeshComponent(Handle<Entity> entity);
    void setEntity(Handle<Entity> entity);
    ~MeshComponent();
    
    void update(DeltaTime dt);

    void setVertices(const std::vector<glm::vec3>& vertices);
    const std::vector<glm::vec3>& vertices() const { return m_vertices; }

    void setUVs(std::vector<glm::vec2> uvs);
    const std::vector<glm::vec2>& uvs() { return m_uvs; }

    void setMaterialRes(std::shared_ptr<MaterialRes> materialRes);
    std::shared_ptr<MaterialRes> materialRes() { return m_materialRes; }
    
    EventBus& eventBus() { return m_eventBus; }

private:
    EventBus m_eventBus;
    Handle<Entity> m_entity;
    Handle<RenderElementFactory> m_renderElementFactory;
    Handle<ResManager<MaterialRes>> m_materialResManager;
    AnyHandle m_renderElement;
    std::shared_ptr<MaterialRes> m_materialRes;
    std::vector<glm::vec3> m_vertices = {{-0.5f,-0.5f, 0.0f},
                                         {-0.5f,0.5f, 0.0f},
                                         {0.5f,-0.5f, 0.0f},
                                         {0.5f,-0.5f, 0.0f},
                                         {-0.5f,0.5f, 0.0f},
                                         {0.5f,0.5f, 0.0f}};

    std::vector<glm::vec2> m_uvs = {{0.0f, 1.0f},
                                   {0.0f, 0.0f},
                                   {1.0f, 1.0f},
                                   {1.0f, 0.0f}};
};

} // flappy
