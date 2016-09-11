#include <string>
#include <iostream>

#include "glshaderprogram.h"
#include "glattribarray.h"

namespace flappy {

using namespace std;

/// The macro prints a build log of shader. It used in constructor and laodShader methods.
#define PRINT_INFO(type, instance) { \
    GLint infoLen = 0; \
    glGet ## type ## iv(instance, GL_INFO_LOG_LENGTH, &infoLen); \
    if (infoLen) { \
        char buf[infoLen]; \
        glGet ## type ## InfoLog(instance, infoLen, NULL, buf); \
        cout << "Could not compile shader:" << endl << buf << endl; \
    } \
}

/// Takes GLSL sources from nullterm strings.
/// Prints logs if build problems.
/// @throw shader_init_failed Initialization filed. See debug output.
GLShaderProgram::GLShaderProgram(const string& vertexSource, const string& fragmentSource) {
    m_vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    m_fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);

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
        }
    }
}

GLShaderProgram::~GLShaderProgram() {
    //TODO: Is it a proper cleanup?
    glDetachShader(m_program, m_vertexShader);
    CHECK_GL_ERROR;
    glDetachShader(m_program, m_fragmentShader);
    CHECK_GL_ERROR;
    glDeleteProgram(m_program);
    CHECK_GL_ERROR;
}

GLShaderProgram::AttribLocation GLShaderProgram::findAttr(const char* name) const {
    AttribLocation result = glGetAttribLocation(getProgram(), name);
    CHECK_GL_ERROR;
    return result;
}

GLShaderProgram::UniformLocation GLShaderProgram::findUniform(const char* name) const {
    UniformLocation result = glGetUniformLocation(getProgram(), name);
    CHECK_GL_ERROR;
    return result;
}

void GLShaderProgram::bind() const {
    glUseProgram(getProgram());
    CHECK_GL_ERROR;
}

void GLShaderProgram::unbind() const {
    glUseProgram(0);
}

/// Render VBOs.
/// @param attribArray VBOs
/// @param uniformFunc Define uniforms here with glUniform...() methods.
/// @see AttribArray
void GLShaderProgram::render(const GLAttribArray & attribArray, function<void()> uniformFunc = [](){}) const {
    bind();
    uniformFunc();
    attribArray.bind();
    glDrawArrays(attribArray.method(), 0, attribArray.size());
    attribArray.unbind();
    unbind();
}

GLuint GLShaderProgram::loadShader(ShaderType shaderType, const string& source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
        throw shader_init_failed();
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
        throw shader_init_failed();
    }
    return shader;
}

} // flappy
