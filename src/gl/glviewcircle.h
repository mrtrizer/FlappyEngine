#pragma once

#include "glviewshape.h"

namespace flappy {

using namespace std;

/// Circle shape (GL_TRIANGLE_FAN, vertexCnt points around the center point)
class GViewCircle: public GLViewShape {
public:
    GViewCircle(int vertexCnt, double r);
    const GLAttribArray & getAttribArray() const override {return circle; }

private:
    int vertexCnt;
    GLAttribArray circle;

    vector<GLTools::Vertex> circleTriangleFan(float r, int count);
};

} // flappy
