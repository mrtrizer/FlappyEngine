#pragma once

#include <memory>

// FIXME: Strange includes
#include <GLView.h>
#include <GLShaderRes.h>
#include <GLAttribArray.h>
#include <GLTools.h>
#include <Color.h>

namespace flappy {

class MeshComponent;

/// Contains a shader for all shapes and draw implementation.
/// All derived classes have to impmelent getAttribArray().
class GLViewShape: public GLView<GLViewShape> {
public:
    GLViewShape(SafePtr<MeshComponent>);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) final;

private:
    GLAttribArray m_attribArray;
    bool m_meshChanged = false;
    SafePtr<MeshComponent> m_meshComponent;
    GLTools::GLColorRGBA m_glColorRGBA;

    GLAttribArray genAttribArray(const std::vector<glm::vec3>& vertices);
};

} // flappy
