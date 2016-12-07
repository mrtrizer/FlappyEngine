#pragma once

#include <memory>

#warning FIXME: Strange includes
#include <GLView.h>
#include <GLShaderProgram.h>
#include <GLAttribArray.h>
#include <GLTools.h>

namespace flappy {

/// Contains a shader for all shapes and draw implementation.
/// All derived classes have to impmelent getAttribArray().
class GLViewShape: public GLView<GLViewShape> {
public:
    GLViewShape();
    void update(const PresenterComponent& presenter) override;
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    virtual const GLAttribArray & getAttribArray() const = 0;

private:
    GLTools::GLColorRGBA m_colorRGBA;
};

} // flappy
