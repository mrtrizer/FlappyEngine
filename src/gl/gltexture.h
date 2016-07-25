#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <vector>

#include <core/texture.h>
#include "gltools.h"
#include "glshaderprogram.h"

/// @brief Holds and manages texture id.
/// @details Auto resizes an image if not fit to
/// w == h == x^2 and saves uv coordinates
/// of image corners.
class GLTexture: public Texture {
public:
    GLTexture(const char * bits, int width, int height);
    ~GLTexture();
    void bind(GLShaderProgram::UniformLocation uniformLoc, int n);

private:
    GLuint texture;
};

#endif // GLTEXTURE_H
