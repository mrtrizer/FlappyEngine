#pragma once

#include "glviewshape.h"

namespace flappy {

/// Rect shape (GL_TRIANGLE_STRIP, two triangles with a common side)
class GLViewRect: public GLViewShape {
public:
    GLViewRect();
    const GLAttribArray & getAttribArray() const override {return m_rect;}

private:
    GLAttribArray m_rect;
};

} // flappy
