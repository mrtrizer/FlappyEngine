#pragma once

#include <glm/glm.hpp>

#include <Component.h>
#include <Render.h>
#include <Entity.h>

#include "MaterialRes.h"
#include "RenderElementFactory.h"

namespace flappy {

class MaterialRes;

class MeshComponent: public Component<MeshComponent> {
public:
    struct MeshChangedEvent : public IEvent {};

    MeshComponent();

    void setVertices(std::vector<glm::vec3> vertices);
    const std::vector<glm::vec3>& vertices() const { return m_vertices; }

    void setUVs(std::vector<glm::vec2> uvs);
    const std::vector<glm::vec2>& uvs() { return m_uvs; }

    void setMaterialRes(std::shared_ptr<MaterialRes> materialRes);
    std::shared_ptr<MaterialRes> materialRes() { return m_materialRes; }

private:
    std::shared_ptr<Render> m_renderElement;
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
