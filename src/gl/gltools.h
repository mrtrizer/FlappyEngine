#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <cstdio>
#include <memory>
#include <cmath>

#ifdef QT_CORE_LIB
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "core/gtools.h"

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
