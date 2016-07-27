#ifndef GLSHAPECIRCLE_H
#define GLSHAPECIRCLE_H

#include "glviewshape.h"

/// Circle shape (GL_TRIANGLE_FAN, vertexCnt points around the center point)
class GViewCircle: public GLViewShape {
public:
    GViewCircle(int vertexCnt, double r);
    const GLAttribArray & getAttribArray() const override {return circle; }

private:
    int vertexCnt;
    GLAttribArray circle;

    std::vector<GLTools::Vertex> circleTriangleFan(float r, int count);
};
#endif // GLSHAPECIRCLE_H
