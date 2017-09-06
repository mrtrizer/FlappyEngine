#pragma once

#include <iostream>
#include <functional>

#include <Res.h>
#include <TextRes.h>

#include "GLTools.h"

namespace flappy {

class GLAttribArray;

class GLShaderProgram: public Res {
public:
    using Program = GLuint;
    using AttribLocation = GLint;
    using UniformLocation = GLint;

    GLShaderProgram(std::shared_ptr<TextRes> vertexShaderRes, std::shared_ptr<TextRes> fragmentShaderRes);
    ~GLShaderProgram();
    void render(const GLAttribArray &, std::function<void()>);
    AttribLocation findAttr(const char*);
    UniformLocation findUniform(const char*);
    Program program() const;
    void initShader();

    std::list<std::shared_ptr<Res>> dependencyList() const override;

protected:
    void bind();
    void unbind() const;

private:
    using ShaderType = GLenum;

    GLuint loadShader(ShaderType, const std::string& source);

    void deinitShader();

    std::shared_ptr<TextRes> m_fragmentShaderRes;
    std::shared_ptr<TextRes> m_vertexShaderRes;

    GLuint m_fragmentShader = 0;
    GLuint m_vertexShader = 0;
    Program m_program = 0;
};

} // flappy
