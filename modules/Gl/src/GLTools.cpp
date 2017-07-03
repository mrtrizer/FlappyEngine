#include <iostream>
#include <exception>
#include <sstream>

#include <Color.h>

#include "GLTools.h"

namespace flappy {

namespace GLTools {

using namespace std;

//TODO: How to get an error description without GLUT?
void checkOpenGLerror(const char * file, const char * func, int line) {
    GLenum errCode = glGetError();
    if(errCode != GL_NO_ERROR) {
        stringstream ss;
        ss << "[ERROR] " << file << ' ' << line << ' ' << func << endl;
        LOGE("%s",ss.str().data());
    }
}

GLColorRGBA::GLColorRGBA(const Color& color):
    r(color.r()),
    g(color.g()),
    b(color.b()),
    a(color.a())
{}

} // GLTools

} // flappy
