#include <cstring>

#include "gltexture.h"
#include "core/gtools.h"

GLTexture::GLTexture(const char *bits, int width, int height):
    uvs({{0,1},{0,0},{1,1},{1,0}})
{
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    CHECK_GL_ERROR;
    glGenTextures(1, &texture);
    CHECK_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, texture);
    CHECK_GL_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    CHECK_GL_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECK_GL_ERROR;

    //check width and height
    if ((width == height) && GTools::isPowOfTwo(width) && GTools::isPowOfTwo(height)) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     static_cast<const GLvoid*>(bits));
        CHECK_GL_ERROR;

    } else {
        int oldWidth = width;
        int oldHeight = height;

        //max and nearest power of two
        int newWidth = 1;
        do {
            newWidth <<= 1;
            oldWidth >>= 1;
            oldHeight >>= 1;
        } while (oldWidth || oldHeight);

        relWidth = (float)width / newWidth;
        relHeight = (float)height / newWidth;

        //recalculate UVs
        uvs[2].u = relWidth;
        uvs[3].u = relWidth;
        uvs[0].v = relHeight;
        uvs[2].v = relHeight;

        //data buffer for square image
        char * newPixBuf = new char[newWidth * newWidth * 4]();
        //image will be located at the top left corner of newPixBuf
        for (int i = 0; i < height; i++)
            std::memcpy(&newPixBuf[i * newWidth * 4], &bits[i * width * 4], width * 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     newWidth, newWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     static_cast<const GLvoid*>(newPixBuf));
        CHECK_GL_ERROR;
        delete []newPixBuf;
    }

    glBindTexture(GL_TEXTURE_2D,0);
    CHECK_GL_ERROR;
}

GLTexture::~GLTexture() {
    glDeleteTextures(1,&texture);
    CHECK_GL_ERROR;
}

void GLTexture::bind(GLShaderProgram::UniformLocation uniformLoc, int n) {
    glActiveTexture(GL_TEXTURE0 + n);
    CHECK_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, texture);
    CHECK_GL_ERROR;
    glUniform1i(uniformLoc, 0);
    CHECK_GL_ERROR;
}


