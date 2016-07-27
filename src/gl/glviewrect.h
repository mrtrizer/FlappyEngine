#ifndef GLSHAPERECT_H
#define GLSHAPERECT_H

#include "glviewshape.h"

/// Rect shape (GL_TRIANGLE_STRIP, two triangles with a common side)
class GViewRect: public GLViewShape {
public:
    GViewRect(float width, float height);
    const GLAttribArray & getAttribArray() const override {return rect;}

private:
    GLAttribArray rect;
};

#endif // GLSHAPERECT_H
