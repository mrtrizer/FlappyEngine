#pragma once

#include <iostream>
#include <functional>

#include "gltools.h"

namespace flappy {

class GLAttribArray;

class GLShaderProgram {
public:
    using Program = GLuint;
    using AttribLocation = GLint;
    using UniformLocation = GLint;

    GLShaderProgram(const string& vertexSource, const string& fragmentSource);
    ~GLShaderProgram();
    void render(const GLAttribArray &, function<void()>) const;
    AttribLocation findAttr(const char*) const;
    UniformLocation findUniform(const char*) const;
    inline Program getProgram() const {return program;}

    class shader_init_failed {};

protected:
    void bind() const;
    void unbind() const;

private:
    using ShaderType = GLenum;

    GLuint fragmentShader;
    GLuint vertexShader;
    Program program;

    GLuint loadShader(ShaderType, const string& source);
};

} // flappy
