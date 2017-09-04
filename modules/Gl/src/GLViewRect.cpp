#include "GLViewRect.h"

namespace flappy {

using namespace std;

GLViewRect::GLViewRect():
    m_rect(GL_TRIANGLE_STRIP)
{
    events()->subscribeIn([this](InitEvent) {
        vector<GLTools::Vertex> vertexList({
                      {-0.5f,-0.5f},
                      {-0.5f,0.5f},
                      {0.5f,-0.5f},
                      {0.5f,0.5f}
                  });

        m_rect.addVBO<GLTools::Vertex>(vertexList, shader()->findAttr("aPosition"));
    });
}

} // flappy
