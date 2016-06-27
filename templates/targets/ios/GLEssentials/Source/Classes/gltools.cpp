#include <iostream>
#include <exception>
#include <sstream>

#include "gltools.h"

namespace GLTools {

//TODO: How to get an error description without GLUT?
void checkOpenGLerror(const char * file, const char * func, int line) {
    GLenum errCode = glGetError();
    if(errCode != GL_NO_ERROR) {
        std::stringstream ss;
        ss << "[ERROR] " << file << ' ' << line << ' ' << func << std::endl;
        LOGE("%s",ss.str().data());
#ifdef QT_DEBUG
        ss << "Text: " << reinterpret_cast<const char *>(gluErrorString(errCode)) << std::endl;
        throw std::runtime_error(ss.str());
#endif
    }
}

}
