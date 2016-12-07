#pragma once

#include <vector>

#include <TextureRes.h>
#warning FIXME: Strange includes
#include <GLTools.h>
#include <GLShaderProgram.h>

namespace flappy {

/// @brief Holds and manages texture id.
/// @details Auto resizes an image if it not fit to
/// w == h == x^2 and saves uv coordinates
/// of image corners.
class GLTexture: public TextureRes {
public:
    GLTexture(const char* bitmapData, int width, int height);
    ~GLTexture();
    void bind(GLShaderProgram::UniformLocation uniformLoc, int n);

private:
    GLuint m_texture;
};

} // flappy
