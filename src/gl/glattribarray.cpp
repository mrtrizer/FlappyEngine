#include "glattribarray.h"

///@param size Count of items for glDrawArrays()
GLAttribArray::GLAttribArray(Method method, Size size):
    size(size),
    method(method){
}

GLAttribArray::~GLAttribArray() {
    for (VBO i: vboBufs) {
        glDeleteBuffers(1,&i.id);
        CHECK_GL_ERROR;
    }
}

void GLAttribArray::bind() const {
    for (auto vbo: vboBufs) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        CHECK_GL_ERROR;
        glVertexAttribPointer(vbo.attr, vbo.componentCount, vbo.itemType, GL_FALSE, 0, 0);
        CHECK_GL_ERROR;
        glEnableVertexAttribArray(vbo.attr);
        CHECK_GL_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLAttribArray::unbind() const {
    for (auto vbo: vboBufs) {
        glDisableVertexAttribArray(vbo.attr);
        CHECK_GL_ERROR;
    }
}
