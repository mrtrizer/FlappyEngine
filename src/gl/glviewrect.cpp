#include "glviewrect.h"

namespace flappy {

using namespace std;

GViewRect::GViewRect():
    m_rect(GL_TRIANGLE_STRIP)
{
    vector<GLTools::Vertex> vertexList({
                  {-0.5f,-0.5f},
                  {-0.5f,0.5f},
                  {0.5f,-0.5f},
                  {0.5f,0.5f}
              });

    m_rect.addVBO<GLTools::Vertex>(vertexList.data(),
                                 static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));
}

} // flappy
