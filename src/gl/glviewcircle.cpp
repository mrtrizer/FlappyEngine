#include "glviewcircle.h"

#include <core/presenter.h>
#include <core/circleshape.h>

namespace flappy {

using namespace std;
using namespace glm;

/// Generates vertices for circle drawing in GL_TRIANGLE_FAN format
/// @param count Count of vertices ( >= 3).
vector<GLTools::Vertex> GViewCircle::circleTriangleFan(float r, int vertexCount)
{
    if (vertexCount < 3)
        throw runtime_error("Too few vertices in circle (has to be >= 3).");
    vector<GLTools::Vertex> vertexList(vertexCount + 2);
    float step = M_PI * 2 / vertexCount;
    vertexList[0] = GLTools::Vertex({0,0});
    for (int i = 0; i < vertexCount + 1; i++)
        vertexList[i + 1] = {cos(step * i) * r, sin(step * i) * r};
    return vertexList;
}

GViewCircle::GViewCircle(int vertexCount):
    circle(GL_TRIANGLE_FAN),
    m_vertexCount(vertexCount)
{
    vector<GLTools::Vertex> vertexList = circleTriangleFan(0.5f, m_vertexCount);
    circle.addVBO<GLTools::Vertex>(vertexList.data(),                           //data array
                                   static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex), //size
                                   GL_FLOAT,                                    //item format
                                   getShader()->findAttr("aPosition")           //attribute id
                                   );
}

} // flappy
