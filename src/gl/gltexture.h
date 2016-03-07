#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <vector>

#include "gltools.h"
#include "glshaderprogram.h"

/// @brief Holds and manages texture id.
/// @details Auto resizes an image if not fit to
/// w == h == x^2 and saves uv coordinates
/// of image corners.
class GLTexture {
public:
    struct UV {
        GLfloat u;
        GLfloat v;
    };

    GLTexture(const char * bits, int width, int height);
    ~GLTexture();
    void bind(GLShaderProgram::UniformLocation uniformLoc, int n);
    inline std::vector<UV> getUVs() const { return uvs; }
    inline float getRelWidth() const {return relWidth;}
    inline float getRelHeight() const {return relHeight;}

private:
    GLuint texture;
    /// Saved UVs after auto resize
    std::vector<UV> uvs;
    /// Relative width (0.0 to 1.0)
    float relWidth = 1.0;
    /// Relative height (0.0 to 1.0)
    float relHeight = 1.0;
};

#endif // GLTEXTURE_H
