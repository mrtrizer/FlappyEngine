#pragma once

#include <vector>

#include "gltools.h"

namespace flappy {

class VBO {
    friend class GLAttribArray;
public:
    ///@param bufSize buf size in bytes
    template<typename ItemType>
    void writeData(const ItemType * buf, int bufSize) {
        using namespace std;
        if (bufSize > m_size)
            throw runtime_error("New buffer is larger than old one.");
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        CHECK_GL_ERROR;
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufSize, buf);
        CHECK_GL_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR;
    }
private:
    int m_itemType;
    int m_componentCount;
    GLint m_attr;
    GLuint m_id;
    int m_size;
};

/// Contains VBOs, draw method and item count for drawArrays().
/// Kinda mesh object.
/// @see Shader
class GLAttribArray {
public:
    using Method = unsigned int;

    explicit GLAttribArray(Method = GL_TRIANGLES, int m_size = -1);
    ~GLAttribArray();

    void bind() const;
    void unbind() const;

    inline int size() const {return m_size;}
    inline Method method() const {return m_method;}

    VBO & vbo(int n) {return m_vboBufs[n];}

    void reset(Method method = GL_TRIANGLES) {
        for (VBO i: m_vboBufs) {
            glDeleteBuffers(1,&i.m_id);
            CHECK_GL_ERROR;
        }
        m_vboBufs.clear();
        m_size = -1;
        m_method = method;
    }

    // TODO: Take vector<ItemType> as first argument
    // TODO: Automatic size determination
    // TODO: Automatic OpenGL type determination
    template<typename ItemType>
    void addVBO(const std::vector<ItemType> vertexList, GLint attr) {
        using namespace std;
        const auto itemType = GL_FLOAT;
        const int bufSize = int(vertexList.size() * sizeof(GLTools::Vertex));
        const ItemType* buf = vertexList.data();
        VBO vbo;
        vbo.m_componentCount = sizeof(ItemType) / 4;
        vbo.m_itemType = itemType;
        vbo.m_attr = attr;
        vbo.m_size = bufSize;
        int count = bufSize / sizeof(ItemType);
        if ((count < m_size) || (m_size == -1)) //I keep min count of attrib items to use in drawArrays
            m_size = count;
        glGenBuffers(1, &vbo.m_id);
        CHECK_GL_ERROR;
        m_vboBufs.push_back(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.m_id);
        CHECK_GL_ERROR;
        glBufferData(GL_ARRAY_BUFFER, bufSize, buf, GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR;
    }

private:
    int m_size;
    std::vector<VBO> m_vboBufs; //need only to cleanup vbos
    Method m_method;
};

} // flappy

