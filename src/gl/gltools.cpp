#include <iostream>
#include <exception>
#include <sstream>

#include "gltools.h"

namespace GLTools {

//TODO: How to get an error description without GLUT?
void checkOpenGLerror(const char * file, const char * func, int line) {
    GLenum errCode = glGetError();
    std::stringstream ss;
    ss << "[ERROR] " << file << ' ' << line << ' ' << func << std::endl;
    if(errCode != GL_NO_ERROR)
        LOGE("%s",ss.str().data());
#ifdef QT_DEBUG
    ss << "Text: " << reinterpret_cast<const char *>(gluErrorString(errCode)) << std::endl;
    if(errCode != GL_NO_ERROR)
        throw std::runtime_error(ss.str());
#endif
}

}
