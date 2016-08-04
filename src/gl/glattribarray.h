#pragma once

#include <vector>

#include "gltools.h"

namespace flappy {

using namespace std;

class VBO {
    friend class GLAttribArray;
public:
    ///@param bufSize buf size in bytes
    template<typename ItemType>
    void writeData(const ItemType * buf, int bufSize) {
        if (bufSize > size)
            throw runtime_error("New buffer is larger than old one.");
        glBindBuffer(GL_ARRAY_BUFFER, id);
        CHECK_GL_ERROR;
        glBufferSubData(GL_ARRAY_BUFFER, 0, bufSize, buf);
        CHECK_GL_ERROR;
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        CHECK_GL_ERROR;
    }
private:
    int itemType;
    int componentCount;
    GLint attr;
    GLuint id;
    int size;
};

/// Contains VBOs, draw method and item count for drawArrays().
/// Kinda mesh object.
/// @see Shader
class GLAttribArray {
public:
    using Method = unsigned int;

    explicit GLAttribArray(Method = GL_TRIANGLES, int size = -1);
    ~GLAttribArray();

    void bind() const;
    void unbind() const;

    inline int getSize() const {return size;}
    inline Method getMethod() const {return method;}

    VBO & getVBO(int n) {return vboBufs[n];}

    template<typename ItemType>
    void addVBO(const ItemType * buf, int bufSize, int itemType, GLint attr) {
        VBO vbo;
        vbo.componentCount = sizeof(ItemType) / 4;
        vbo.itemType = itemType;
        vbo.attr = attr;
        vbo.size = bufSize;
        int count = bufSize / sizeof(ItemType);
        if ((count < size) || (size == -1)) //I keep min count of attrib items to use in drawArrays
            size = count;
        glGenBuffers(1, &vbo.id);
        CHECK_GL_ERROR;
        vboBufs.push_back(vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo.id);
        CHECK_GL_ERROR;
        glBufferData(GL_ARRAY_BUFFER, bufSize, buf, GL_DYNAMIC_DRAW);
        CHECK_GL_ERROR;
        if (itemType != GL_FLOAT)
            runtime_error("Now only GL_FLOAT items supported!");
    }

private:
    using VBOBufs = vector<VBO>;
    GLuint id;
    int size;
    VBOBufs vboBufs; //need only to cleanup vbos
    Method method;
};

} // flappy

