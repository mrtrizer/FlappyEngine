#pragma once

#include <cstdio>
#include <memory>
#include <cmath>

#include <GLIncludes.h>

#include <Tools.h>


namespace flappy {

class Color;

/// Contains tools for work with OpenGL
namespace GLTools {

    struct Vertex {
      GLfloat x;
      GLfloat y;
      GLfloat z;
    };

    struct GLColorRGBA {
        GLColorRGBA() = default;
        explicit GLColorRGBA(const Color& color);
        GLfloat r = 1.0f;
        GLfloat g = 1.0f;
        GLfloat b = 1.0f;
        GLfloat a = 1.0f;
    };

    void checkOpenGLerror(const char * = "", const char * = "", int = 0);

}

} // flappy

#define CHECK_GL_ERROR GLTools::checkOpenGLerror(__FILE__, __FUNCTION__, __LINE__)
