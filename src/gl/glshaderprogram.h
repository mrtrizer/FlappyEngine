#pragma once

#include <iostream>
#include <functional>

#include "gltools.h"

namespace flappy {

class GLAttribArray;

class GLShaderProgram {
public:
    typedef const char * VertexSource;
    typedef const char * FragmentSource;
    typedef GLuint Program;
    typedef GLint AttribLocation;
    typedef GLint UniformLocation;
    typedef const char * Name;
    typedef const function<void()>& UniformFunc;

    GLShaderProgram(VertexSource, FragmentSource);
    ~GLShaderProgram();
    void render(const GLAttribArray &, UniformFunc) const;
    AttribLocation findAttr(Name) const;
    UniformLocation findUniform(Name) const;
    inline Program getProgram() const {return program;}

    class shader_init_failed {};

protected:
    void bind() const;
    void unbind() const;

private:
    typedef GLenum ShaderType;
    typedef const char * ShaderSource;

    GLuint fragmentShader;
    GLuint vertexShader;
    Program program;

    GLuint loadShader(ShaderType, ShaderSource);
};

} // flappy
