#include "GLShaderRes.h"

#include <string>
#include <iostream>

#include <IGLManager.h>
#include <Entity.hpp>

#include "GLAttribArray.h"

namespace flappy {

using namespace std;

/// The macro prints a build log of shader. It used in constructor and laodShader methods.
#define PRINT_INFO(type, instance) { \
    GLint infoLen = 0; \
    glGet ## type ## iv(instance, GL_INFO_LOG_LENGTH, &infoLen); \
    if (infoLen) { \
        char buf[infoLen]; \
        glGet ## type ## InfoLog(instance, infoLen, NULL, buf); \
        cout << "Shader compilation error:" << endl << buf << endl; \
    } \
}

/// Takes GLSL sources from nullterm strings.
/// Prints logs if build problems.
/// @throw shader_init_failed Initialization filed. See debug output.
GLShaderRes::GLShaderRes(Handle<Entity> rootEntity, string vertexShaderStr, string fragmentShaderStr)
    : m_fragmentShaderStr(fragmentShaderStr)
    , m_vertexShaderStr(vertexShaderStr)
    , m_rootEntity(rootEntity)
{
    initShader();
}

GLShaderRes::~GLShaderRes() {
    deinitShader();
}

GLuint GLShaderRes::loadShader(ShaderType shaderType, const string& source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
        throw std::runtime_error("Shader creation error");
    auto sourceStrPtr = source.c_str();
    glShaderSource(shader, 1, &sourceStrPtr, NULL);
    CHECK_GL_ERROR;
    glCompileShader(shader);
    CHECK_GL_ERROR;
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        PRINT_INFO(Shader, shader);
        glDeleteShader(shader);
        throw std::runtime_error("Shader compilation error");
    }
    return shader;
}

void GLShaderRes::initShader() {
    deinitShader();

    m_vertexShader = loadShader(GL_VERTEX_SHADER, m_vertexShaderStr);
    m_fragmentShader = loadShader(GL_FRAGMENT_SHADER, m_fragmentShaderStr);

    m_program = glCreateProgram();
    if (m_program) {
        glAttachShader(m_program, m_vertexShader);
        CHECK_GL_ERROR;
        glAttachShader(m_program, m_fragmentShader);
        CHECK_GL_ERROR;
        glLinkProgram(m_program);
        CHECK_GL_ERROR;
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            PRINT_INFO(Program, m_program);
            glDeleteProgram(m_program);
            m_program = 0;
            throw std::runtime_error("Shader program linking error");
        }
    }
}

void GLShaderRes::deinitShader() {
    if (m_program == 0)
        return;
    if (m_vertexShader != 0) {
        glDetachShader(m_program, m_vertexShader);
        CHECK_GL_ERROR;
        m_vertexShader = 0;
    }
    if (m_fragmentShader != 0) {
        glDetachShader(m_program, m_fragmentShader);
        CHECK_GL_ERROR;
        m_fragmentShader = 0;
    }
    glDeleteProgram(m_program);
    CHECK_GL_ERROR;
    m_program = 0;
}

GLShaderRes::AttribLocation GLShaderRes::findAttr(const char* name) {
    AttribLocation result = glGetAttribLocation(program(), name);
    CHECK_GL_ERROR;
    return result;
}

GLShaderRes::UniformLocation GLShaderRes::findUniform(const char* name) {
    UniformLocation result = glGetUniformLocation(program(), name);
    CHECK_GL_ERROR;
    return result;
}

GLShaderRes::Program GLShaderRes::program() const
{
    return m_program;
}

void GLShaderRes::bind() {
    glUseProgram(program());
    CHECK_GL_ERROR;
}

void GLShaderRes::unbind() const {
    glUseProgram(0);
}

/// Render VBOs.
/// @param attribArray VBOs
/// @param uniformFunc Define uniforms here with glUniform...() methods.
/// @see AttribArray
void GLShaderRes::render(const GLAttribArray & attribArray, function<void()> uniformFunc = [](){}) {
    bind();
    uniformFunc();
    attribArray.bind();
    glDrawArrays(attribArray.method(), 0, attribArray.size());
    attribArray.unbind();
    unbind();
}

} // flappy
