#pragma once

#include <iostream>
#include <functional>

#include <ShaderRes.h>
#include <TextRes.h>
#include <Handle.hpp>
#include <ISubscription.h>

#include "GLTools.h"

namespace flappy {

class GLAttribArray;
class Entity;

class GLShaderRes: public ShaderRes {
public:
    using Program = GLuint;
    using AttribLocation = GLint;
    using UniformLocation = GLint;

    GLShaderRes(Handle<Entity> rootEntity, std::string vertexShaderRes, std::string fragmentShaderStr);
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
    Handle<Entity> m_rootEntity;
    std::shared_ptr<ISubscription> m_initSubscription;
    std::shared_ptr<ISubscription> m_deinitSubscription;

    GLuint m_fragmentShader = 0;
    GLuint m_vertexShader = 0;
    Program m_program = 0;
};

} // flappy
