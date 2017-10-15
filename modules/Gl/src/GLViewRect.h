#pragma once

#include "GLViewShape.h"

namespace flappy {

class MeshComponent;

/// Rect shape (GL_TRIANGLE_STRIP, two triangles with a common side)
class GLViewRect: public GLViewShape {
public:

    GLViewRect(SafePtr<MeshComponent> meshComponent);
    const GLAttribArray & getAttribArray() const final {return m_rect;}

private:
    GLAttribArray m_rect;
};

} // flappy
