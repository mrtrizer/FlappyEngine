#include <glm/gtc/type_ptr.hpp>
#include <PresenterComponent.h>

#include "GLViewShape.h"

namespace flappy {

using namespace glm;

static const char shapeVShader[] =
    "attribute vec2 aPosition;\n"
    "uniform mat4 uMVMatrix;\n"
    "uniform mat4 uPMatrix;\n"
    "varying vec4 vColor;\n"
    "void main() {\n"
    "   gl_Position = uPMatrix * uMVMatrix * vec4(aPosition,0,1);\n"
    "}\n";

static const char shapeFShader[] =
#if defined(GL_ES) || defined(EMSCRIPTEN) || TARGET_OS_IPHONE == 1
    "precision mediump float;\n"
#endif
    "uniform vec4 uColor;\n"
    "void main() {\n"
    "   gl_FragColor = uColor;\n"
    "}\n";

GLViewShape::GLViewShape() :
    GLView(shapeVShader, shapeFShader)
{}

void GLViewShape::update(const PresenterComponent& presenter) {
    auto& shape = static_cast<const RectShape&>(presenter);
    m_colorRGBA = GLTools::GLColorRGBA(shape.color());
}

void GLViewShape::draw(const mat4 &pMartrix, const mat4 &mvMatrix) {
    getShader()->render(getAttribArray(), [this, mvMatrix, pMartrix](){
        glUniformMatrix4fv(getShader()->findUniform("uMVMatrix"),1,false,value_ptr(mvMatrix));
        glUniformMatrix4fv(getShader()->findUniform("uPMatrix"),1,false,value_ptr(pMartrix));
        glUniform4fv(getShader()->findUniform("uColor"),1, reinterpret_cast<GLfloat *>(&m_colorRGBA));
    });
}

} // flappy
