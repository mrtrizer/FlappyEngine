#include "glviewshape.h"

static const char shapeVShader[] =
    "attribute vec2 aPosition;\n"
    "uniform mat4 uMVMatrix;\n"
    "uniform mat4 uPMatrix;\n"
    "varying vec4 vColor;\n"
    "void main() {\n"
    "   gl_Position = uPMatrix * uMVMatrix * vec4(aPosition,0,1);\n"
    "}\n";

static const char shapeFShader[] =
    "precision mediump float;\n"
    "uniform vec4 uColor;\n"
    "void main() {\n"
    "   gl_FragColor = uColor;\n"
    "}\n";

GLViewShape::GLViewShape() :
    GLView(shapeVShader, shapeFShader),
    colorRGBA({1.0f, 1.0f, 1.0f, 1.0f}){

}

/// Generates vertices for circle drawing in GL_TRIANGLE_FAN format
/// @param count Count of vertices ( >= 3).
std::vector<GLTools::Vertex> GViewCircle::circleTriangleFan(float r, int count) {
    if (count < 3)
        throw std::runtime_error("Too few vertices in circle (has to be >= 3).");
    std::vector<GLTools::Vertex> vertexList(count + 2);
    float step = M_PI * 2 / count;
    vertexList[0] = GLTools::Vertex({0,0});
    for (int i = 0; i < count + 1; i++)
        vertexList[i + 1] = {std::cos(step * i) * r, std::sin(step * i) * r};
    return vertexList;
}

void GLViewShape::draw(const PMatrix pMatrix, const MVMatrix mvMatrix) {
    getShader()->render(getAttribArray(), [this, mvMatrix, pMatrix](){
        glUniformMatrix4fv(getShader()->findUniform("uMVMatrix"),1,false,mvMatrix);
        glUniformMatrix4fv(getShader()->findUniform("uPMatrix"),1,false,pMatrix);
        glUniform4fv(getShader()->findUniform("uColor"),1, reinterpret_cast<GLfloat *>(&colorRGBA));
    });
}

GViewCircle::GViewCircle(int vertexCnt, double r):
    vertexCnt(vertexCnt),
    circle(GL_TRIANGLE_FAN){
    std::vector<GLTools::Vertex> vertexList = circleTriangleFan(r,vertexCnt);
    circle.addVBO<GLTools::Vertex>(vertexList.data(),                           //data array
                                   vertexList.size() * sizeof(GLTools::Vertex), //size
                                   GL_FLOAT,                                    //item format
                                   getShader()->findAttr("aPosition")            //attribute id
                                   );
}

GViewRect::GViewRect(float width, float height):
    rect(GL_TRIANGLE_STRIP){

    std::vector<GLTools::Vertex> vertexList({
                                                {0,0},
                                                {0,height},
                                                {width,0},
                                                {width,height}
                                            });
    rect.addVBO<GLTools::Vertex>(vertexList.data(),
                                 vertexList.size() * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));
}
