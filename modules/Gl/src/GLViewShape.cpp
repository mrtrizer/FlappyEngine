#include <glm/gtc/type_ptr.hpp>

#include "GLViewShape.h"

namespace flappy {

using namespace glm;

void GLViewShape::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    m_glColorRGBA = GLTools::GLColorRGBA(m_color);
    shader()->render(getAttribArray(), [this, mvMatrix, pMartrix](){
        glUniformMatrix4fv(shader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
        glUniformMatrix4fv(shader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
        glUniform4fv(shader()->findUniform("uColor"),1, reinterpret_cast<GLfloat *>(&m_glColorRGBA));
    });
}

} // flappy
