#pragma once

#include "glviewshape.h"

namespace flappy {

/// Circle shape (GL_TRIANGLE_FAN, vertexCnt points around the center point)
class GViewCircle: public GLViewShape {
public:
    GViewCircle(int vertexCount);
    const GLAttribArray& getAttribArray() const override {return circle; }

private:
    GLAttribArray circle;
    int m_vertexCount = 32;

    std::vector<GLTools::Vertex> circleTriangleFan(float r, int vertexCount);
};

} // flappy
