#pragma once

#include "GLViewShape.h"

namespace flappy {

/// Circle shape (GL_TRIANGLE_FAN, vertexCnt points around the center point)
class GLViewCircle: public GLViewShape {
public:
    GLViewCircle(int vertexCount = 32);
    const GLAttribArray& getAttribArray() const override {return m_circle; }

private:
    GLAttribArray m_circle;
    std::shared_ptr<GLShaderProgram> m_glShaderProgramRes;
    int m_vertexCount = 32;

    std::vector<GLTools::Vertex> circleTriangleFan(float r, int vertexCount);
};

} // flappy
