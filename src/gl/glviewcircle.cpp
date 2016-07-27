#include "glviewcircle.h"

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

GViewCircle::GViewCircle(int vertexCnt, double r):
    vertexCnt(vertexCnt),
    circle(GL_TRIANGLE_FAN){
    std::vector<GLTools::Vertex> vertexList = circleTriangleFan(r,vertexCnt);
    circle.addVBO<GLTools::Vertex>(vertexList.data(),                           //data array
                                   static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex), //size
                                   GL_FLOAT,                                    //item format
                                   getShader()->findAttr("aPosition")           //attribute id
                                   );
}
