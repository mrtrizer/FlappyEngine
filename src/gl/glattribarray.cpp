#include "glattribarray.h"

namespace flappy {

///@param size Count of items for glDrawArrays()
GLAttribArray::GLAttribArray(Method method, int size):
    m_size(size),
    m_method(method){
}

GLAttribArray::~GLAttribArray() {
    reset();
}

void GLAttribArray::bind() const {
    for (auto vbo: m_vboBufs) {
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
    for (auto vbo: m_vboBufs) {
        glDisableVertexAttribArray(vbo.attr);
        CHECK_GL_ERROR;
    }
}

} // flappy
