#include "GLViewRect.h"

#include <glm/glm.hpp>

#include <MeshComponent.h>

namespace flappy {

using namespace std;

GLViewRect::GLViewRect(SafePtr<MeshComponent> meshComponent):
    m_rect(GL_TRIANGLE_STRIP)
{
    addDependency(MeshComponent::id());

    subscribe([this, meshComponent](InitEvent) {
        const auto& vertices = meshComponent->vertices();
        vector<GLTools::Vertex> glVertices(vertices.size());

        std::transform(vertices.begin(), vertices.end(), glVertices.begin(), [](glm::vec3 vertex) {
            return GLTools::Vertex{vertex.x, vertex.y, vertex.z};
        });

        m_rect.addVBO<GLTools::Vertex>(glVertices, shader()->findAttr("aPosition"));
    });
}

} // flappy
