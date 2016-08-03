#include <iostream>
#include <exception>
#include <sstream>

#include "gltools.h"

namespace flappy {

namespace GLTools {

//TODO: How to get an error description without GLUT?
void checkOpenGLerror(const char * file, const char * func, int line) {
    GLenum errCode = glGetError();
    if(errCode != GL_NO_ERROR) {
        stringstream ss;
        ss << "[ERROR] " << file << ' ' << line << ' ' << func << endl;
        LOGE("%s",ss.str().data());
#ifdef QT_DEBUG
        ss << "Text: " << reinterpret_cast<const char *>(gluErrorString(errCode)) << endl;
        throw runtime_error(ss.str());
#endif
    }
}

} // GLTools

} // flappy
