#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <cstdio>
#include <memory>
#include <cmath>

#define TARGET_NOT_SUPPORTED "Target platform is not suported yet"

#if defined(_WIN32)
  #error TARGET_NOT_SUPPORTED
#elif defined(__APPLE__)
  #include "TargetConditionals.h"
  #if TARGET_OS_IPHONE == 1
    #include <OpenGLES/ES2/gl.h>
    #include <OpenGLES/ES2/glext.h>
  #elif TARGET_OS_MAC == 1
    #include <OpenGL/GL.h>
  #else
    #error TARGET_NOT_SUPPORTED
  #endif
#elif defined(ANDROID_JNI)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
#elif defined(__unix__)
  #include <GL/glew.h>
#endif

#include <core/gtools.h>

/// Contains tools for work with OpenGL
namespace GLTools {

typedef float R;

struct Vertex {
  GLfloat x;
  GLfloat y;
};

void checkOpenGLerror(const char * = "", const char * = "", int = 0);

}

#define CHECK_GL_ERROR GLTools::checkOpenGLerror(__FILE__, __FUNCTION__, __LINE__)

#endif // GLTOOLS_H
