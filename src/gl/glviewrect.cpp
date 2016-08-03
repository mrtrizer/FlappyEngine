#include "glviewrect.h"

namespace flappy {

GViewRect::GViewRect(float width, float height):
    rect(GL_TRIANGLE_STRIP){

    vector<GLTools::Vertex> vertexList({
                  {-width / 2,-height / 2},
                  {-width / 2,height / 2},
                  {width / 2,-height / 2},
                  {width / 2,height / 2}
              });
    rect.addVBO<GLTools::Vertex>(vertexList.data(),
                                 static_cast<int>(vertexList.size()) * sizeof(GLTools::Vertex),
                                 GL_FLOAT,
                                 getShader()->findAttr("aPosition"));
}

} // flappy
