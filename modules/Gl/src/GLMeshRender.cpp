#include "GLMeshRender.h"

#include <glm/gtc/type_ptr.hpp>

#include <MeshComponent.h>
#include <MaterialRes.h>

#include "GLTextureRes.h"

namespace flappy {

GLMeshRender::GLMeshRender(SafePtr<MeshComponent> meshComponent):
    m_attribArray(GL_TRIANGLE_STRIP)
{
    addDependency(MeshComponent::id());

    m_meshComponent = meshComponent;

    subscribe([this, meshComponent](InitEvent) {
        m_attribArray = genAttribArray();
        m_meshChanged = false;
    });

    subscribe([this](MeshComponent::MeshChangedEvent) {
        m_meshChanged = true;
    });
}

GLAttribArray GLMeshRender::genAttribArray() {
    auto& vertices = m_meshComponent->vertices();
    std::vector<GLTools::Vertex> glVertices(vertices.size());

    std::transform(vertices.begin(), vertices.end(), glVertices.begin(), [](glm::vec3 vertex) {
        return GLTools::Vertex{vertex.x, vertex.y, vertex.z};
    });

    GLAttribArray attribArray(GL_TRIANGLES);
    attribArray.addVBO<GLTools::Vertex>(glVertices, shader()->findAttr("aPosition"));

    GLint textureAttr = shader()->findAttr("aTexCoord");
    if (textureAttr != -1) {
        auto& uvs = m_meshComponent->uvs();
        std::vector<GLTextureRes::UV> glUVs(uvs.size());
        std::transform(uvs.begin(), uvs.end(), glUVs.begin(), [](glm::vec2 pos) {
            return GLTextureRes::UV{pos.x, pos.y};
        });

        attribArray.addVBO<GLTextureRes::UV>(glUVs, textureAttr);
    }

    return attribArray;
}

void GLMeshRender::draw(const glm::mat4 &pMartrix, const glm::mat4 &mvMatrix) {
    if (m_meshChanged) {
        m_attribArray = genAttribArray();
        m_meshChanged = false;
    }

    shader()->render(m_attribArray, [this, mvMatrix, pMartrix](){
        glUniformMatrix4fv(shader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
        glUniformMatrix4fv(shader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
        auto materialRes = m_meshComponent->materialRes();
        for (auto uniformPair : materialRes->vec4Map()) {
            GLint uniform = shader()->findUniform(uniformPair.first.c_str());
            if (uniform != -1)
                glUniform4fv(uniform, 1, glm::value_ptr(uniformPair.second));
        }
        for (auto uniformPair : materialRes->textureResMap()) {
            GLint uniform = shader()->findUniform(uniformPair.first.c_str());
            if (uniform != -1) {
                auto glTexture = std::static_pointer_cast<GLTextureRes>(uniformPair.second);
                glTexture->bind(uniform, 0);
            }
        }
    });
}

} // flappy
