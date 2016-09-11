#include "glviewrect.h"

namespace flappy {

using namespace std;

GLViewRect::GLViewRect():
    m_rect(GL_TRIANGLE_STRIP)
{
    vector<GLTools::Vertex> vertexList({
                  {-0.5f,-0.5f},
                  {-0.5f,0.5f},
                  {0.5f,-0.5f},
                  {0.5f,0.5f}
              });

    m_rect.addVBO<GLTools::Vertex>(vertexList, getShader()->findAttr("aPosition"));
}

} // flappy
