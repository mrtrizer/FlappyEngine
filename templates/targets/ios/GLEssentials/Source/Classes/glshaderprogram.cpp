#include <string>
#include <iostream>

#include "glshaderprogram.h"
#include "glattribarray.h"

using namespace std;

/// The macro prints a build log of shader. It used in constructor and laodShader methods.
#define PRINT_INFO(type, instance) { \
    GLint infoLen = 0; \
    glGet ## type ## iv(instance, GL_INFO_LOG_LENGTH, &infoLen); \
    if (infoLen) { \
        char buf[infoLen]; \
        glGet ## type ## InfoLog(instance, infoLen, NULL, buf); \
        std::cout << "Could not compile shader:" << std::endl << buf << std::endl; \
    } \
}

/// Takes GLSL sources from nullterm strings.
/// Prints logs if build problems.
/// @throw shader_init_failed Initialization filed. See debug output.
GLShaderProgram::GLShaderProgram(VertexSource vertexSource, FragmentSource fragmentSource) {
    vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);

    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        CHECK_GL_ERROR;
        glAttachShader(program, fragmentShader);
        CHECK_GL_ERROR;
        glLinkProgram(program);
        CHECK_GL_ERROR;
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            PRINT_INFO(Program, program);
            glDeleteProgram(program);
            program = 0;
        }
    }
}

GLShaderProgram::~GLShaderProgram() {
    //TODO: Is it a proper cleanup?
    glDetachShader(program, vertexShader);
    CHECK_GL_ERROR;
    glDetachShader(program, fragmentShader);
    CHECK_GL_ERROR;
    glDeleteProgram(program);
    CHECK_GL_ERROR;
}

GLShaderProgram::AttribLocation GLShaderProgram::findAttr(Name name) const {
    AttribLocation result = glGetAttribLocation(getProgram(), name);
    CHECK_GL_ERROR;
    return result;
}

GLShaderProgram::UniformLocation GLShaderProgram::findUniform(Name name) const {
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
void GLShaderProgram::render(const GLAttribArray & attribArray, UniformFunc uniformFunc = [](){}) const {
    bind();
    uniformFunc();
    attribArray.bind();
    glDrawArrays(attribArray.getMethod(), 0, attribArray.getSize());
    attribArray.unbind();
    unbind();
}

GLuint GLShaderProgram::loadShader(ShaderType shaderType, ShaderSource source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
        throw shader_init_failed();
    glShaderSource(shader, 1, &source, NULL);
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
