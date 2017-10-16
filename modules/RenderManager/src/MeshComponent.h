#pragma once

#include <glm/glm.hpp>

#include <Component.h>
#include <View.h>
#include <Entity.h>

#include "RenderElementFactory.h"

namespace flappy {

class MeshComponent: public Component<MeshComponent> {
public:
    struct MeshChangedEvent : public IEvent {};

    MeshComponent();

    void setVertices(std::vector<glm::vec3> vertices);
    const std::vector<glm::vec3>& vertices() const { return m_vertices; }

    void setColor(Color color);
    Color color() { return m_color; }

private:
    std::shared_ptr<View> m_renderElement;
    std::vector<glm::vec3> m_vertices = {{-0.5f,-0.5f, 0.0f},
                                         {-0.5f,0.5f, 0.0f},
                                         {0.5f,-0.5f, 0.0f},
                                         {0.5f,-0.5f, 0.0f},
                                         {-0.5f,0.5f, 0.0f},
                                         {0.5f,0.5f, 0.0f}};
    Color m_color;
};

} // flappy
