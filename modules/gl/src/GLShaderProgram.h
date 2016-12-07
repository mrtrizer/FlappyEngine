#pragma once

#include <iostream>
#include <functional>

#include "GLTools.h"

namespace flappy {

class GLAttribArray;

class GLShaderProgram {
public:
    using Program = GLuint;
    using AttribLocation = GLint;
    using UniformLocation = GLint;

    GLShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
    ~GLShaderProgram();
    void render(const GLAttribArray &, std::function<void()>) const;
    AttribLocation findAttr(const char*) const;
    UniformLocation findUniform(const char*) const;
    inline Program getProgram() const {return m_program;}

    class shader_init_failed {};

protected:
    void bind() const;
    void unbind() const;

private:
    using ShaderType = GLenum;

    GLuint m_fragmentShader;
    GLuint m_vertexShader;
    Program m_program;

    GLuint loadShader(ShaderType, const std::string& source);
};

} // flappy
