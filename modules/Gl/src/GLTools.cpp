#include <iostream>
#include <exception>
#include <sstream>
#include <map>

#include <Color.h>

#include "GLTools.h"

namespace flappy {

namespace GLTools {

using namespace std;

void checkOpenGLerror(const char * file, const char * func, int line) {
    std::map<GLenum, std::string> errCodeMap {
        {GL_INVALID_ENUM, "Unknown error"},
        {GL_INVALID_VALUE, "Invalid value"},
        {GL_INVALID_OPERATION, "Invalid operation"},
        {GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid framebuffer operation"},
        {GL_OUT_OF_MEMORY, "Out of memory"},
        {GL_STACK_UNDERFLOW, "Stack underflow"},
        {GL_STACK_OVERFLOW, "Stack overflow"},
    };
    GLenum errCode = glGetError();
    if(errCode != GL_NO_ERROR) {
        stringstream ss;
        ss << " OpenGL error " << errCodeMap[errCode] << "\nin " << file << ' ' << line << ' ' << func <<  endl;
        throw std::runtime_error(ss.str());
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
