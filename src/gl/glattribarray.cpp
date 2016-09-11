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
        glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id);
        CHECK_GL_ERROR;
        glVertexAttribPointer(vbo.m_attr, vbo.m_componentCount, vbo.m_itemType, GL_FALSE, 0, 0);
        CHECK_GL_ERROR;
        glEnableVertexAttribArray(vbo.m_attr);
        CHECK_GL_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GLAttribArray::unbind() const {
    for (auto vbo: m_vboBufs) {
        glDisableVertexAttribArray(vbo.m_attr);
        CHECK_GL_ERROR;
    }
}

} // flappy
