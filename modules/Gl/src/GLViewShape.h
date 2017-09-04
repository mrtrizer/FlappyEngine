#pragma once

#include <memory>

// FIXME: Strange includes
#include <GLView.h>
#include <GLShaderProgram.h>
#include <GLAttribArray.h>
#include <GLTools.h>
#include <Color.h>

namespace flappy {

/// Contains a shader for all shapes and draw implementation.
/// All derived classes have to impmelent getAttribArray().
class GLViewShape: public GLView<GLViewShape> {
public:
    using GLView::GLView;

    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    virtual const GLAttribArray & getAttribArray() const = 0;
    void setColor(Color color) { m_color = color; }
    Color color() { return m_color; }

private:

    GLTools::GLColorRGBA m_glColorRGBA;
    Color m_color;
};

} // flappy
