#pragma once

#include "glviewshape.h"

namespace flappy {

/// Rect shape (GL_TRIANGLE_STRIP, two triangles with a common side)
class GViewRect: public GLViewShape {
public:
    GViewRect();
    const GLAttribArray & getAttribArray() const override {return m_rect;}

private:
    GLAttribArray m_rect;
};

} // flappy
