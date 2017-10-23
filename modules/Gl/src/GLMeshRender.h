#pragma once

#include <memory>

// FIXME: Strange includes
#include <GLRender.h>
#include <GLShaderRes.h>
#include <GLAttribArray.h>
#include <GLTools.h>
#include <Color.h>
#include <MaterialRes.h>

namespace flappy {

class MeshComponent;

/// Contains a shader for all shapes and draw implementation.
/// All derived classes have to impmelent getAttribArray().
class GLMeshRender: public GLRender<GLMeshRender> {
public:
    GLMeshRender(SafePtr<MeshComponent>);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) final;

private:
    GLAttribArray m_attribArray;
    bool m_meshChanged = false;
    SafePtr<MeshComponent> m_meshComponent;
    static const std::unordered_map<MaterialRes::RenderMode, GLenum> m_glRenderModes;

    GLAttribArray genAttribArray();
};

} // flappy
