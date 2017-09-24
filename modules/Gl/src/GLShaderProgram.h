#pragma once

#include <iostream>
#include <functional>

#include <Res.h>
#include <TextRes.h>

#include "GLTools.h"

namespace flappy {

class GLAttribArray;

class GLShaderRes: public Res<GLShaderRes> {
public:
    using Program = GLuint;
    using AttribLocation = GLint;
    using UniformLocation = GLint;

    GLShaderRes(std::string vertexShaderRes, std::string fragmentShaderStr);
    ~GLShaderRes();
    void render(const GLAttribArray &, std::function<void()>);
    AttribLocation findAttr(const char*);
    UniformLocation findUniform(const char*);
    Program program() const;
    void initShader();

protected:
    void bind();
    void unbind() const;

private:
    using ShaderType = GLenum;

    GLuint loadShader(ShaderType, const std::string& source);

    void deinitShader();

    std::string m_fragmentShaderStr;
    std::string m_vertexShaderStr;

    GLuint m_fragmentShader = 0;
    GLuint m_vertexShader = 0;
    Program m_program = 0;
};

} // flappy
