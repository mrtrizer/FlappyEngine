#include <cstring>

#include <Tools.h>

#include "GLTextureRes.h"

namespace flappy {

GLTexture::GLTexture(const char *bitmapData, int width, int height):
    TextureRes({width, height})
{
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    CHECK_GL_ERROR;
    glGenTextures(1, &m_texture);
    CHECK_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, m_texture);
    CHECK_GL_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    CHECK_GL_ERROR;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECK_GL_ERROR;

    //check width and height
    if ((width == height) && Tools::isPowOfTwo(width) && Tools::isPowOfTwo(height)) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     width, width, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     static_cast<const GLvoid*>(bitmapData));
        CHECK_GL_ERROR;

    } else {
        int powTwoWidth = Tools::nextHighestPowOfTwo32(width);
        int powTwoHeight = Tools::nextHighestPowOfTwo32(height);

        int maxPowTwo = std::max(powTwoWidth, powTwoHeight);

        m_relWidth = (float)width / maxPowTwo;
        m_relHeight = (float)height / maxPowTwo;

        //recalculate UVs
        m_uvs[2].u = m_relWidth;
        m_uvs[3].u = m_relWidth;
        m_uvs[0].v = m_relHeight;
        m_uvs[2].v = m_relHeight;

        //data buffer for square image
        char * newPixBuf = new char[maxPowTwo * maxPowTwo * 4]();
        //image will be located at the top left corner of newPixBuf
        for (int i = 0; i < height; i++)
            memcpy(&newPixBuf[i * maxPowTwo * 4], &bitmapData[i * width * 4], width * 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     maxPowTwo, maxPowTwo, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     static_cast<const GLvoid*>(newPixBuf));
        CHECK_GL_ERROR;
        delete []newPixBuf;
    }

    glBindTexture(GL_TEXTURE_2D,0);
    CHECK_GL_ERROR;
}

GLTexture::~GLTexture() {
    glDeleteTextures(1,&m_texture);
    CHECK_GL_ERROR;
}

void GLTexture::bind(GLShaderProgram::UniformLocation uniformLoc, int n) {
    glActiveTexture(GL_TEXTURE0 + n);
    CHECK_GL_ERROR;
    glBindTexture(GL_TEXTURE_2D, m_texture);
    CHECK_GL_ERROR;
    glUniform1i(uniformLoc, 0);
    CHECK_GL_ERROR;
}

} // flappy
