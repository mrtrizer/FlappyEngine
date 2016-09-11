#include "glviewrect.h"

namespace flappy {

using namespace std;

GViewRect::GViewRect():
    rect(GL_TRIANGLE_STRIP)
{
    vector<GLTools::Vertex> vertexList({
                  {-0.5f,-0.5f},
                  {-0.5f,0.5f},
                  {0.5f,-0.5f},
                  {0.5f,0.5f}
              });

    rect.addVBO<GLTools::Vertex>(vertexList.data(),
                                 static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));
}

} // flappy
