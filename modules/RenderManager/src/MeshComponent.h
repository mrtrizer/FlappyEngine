#pragma once

#include <glm/glm.hpp>

#include <Component.h>
#include <Render.h>
#include <Entity.h>

#include "RenderElementFactory.h"

namespace flappy {

class MaterialRes;

class MeshComponent: public Component<MeshComponent> {
public:
    struct MeshChangedEvent : public IEvent {};

    MeshComponent();

    void setVertices(std::vector<glm::vec3> vertices);
    const std::vector<glm::vec3>& vertices() const { return m_vertices; }

    void setMaterialRes(std::shared_ptr<MaterialRes> materialRes);
    std::shared_ptr<MaterialRes> materialRes() { return m_materialRes; }

    void setColor(Color color);
    Color color() { return m_color; }

private:
    std::shared_ptr<Render> m_renderElement;
    std::shared_ptr<MaterialRes> m_materialRes;
    std::vector<glm::vec3> m_vertices = {{-0.5f,-0.5f, 0.0f},
                                         {-0.5f,0.5f, 0.0f},
                                         {0.5f,-0.5f, 0.0f},
                                         {0.5f,-0.5f, 0.0f},
                                         {-0.5f,0.5f, 0.0f},
                                         {0.5f,0.5f, 0.0f}};
    Color m_color;
};

} // flappy
