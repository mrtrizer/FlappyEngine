#pragma once

#include <vector>

#include <TextureRes.h>
// FIXME: Strange includes
#include <GLTools.h>
#include <GLShaderRes.h>
#include <SafePtr.h>


namespace flappy {

class IRgbaBitmapRes;
class Entity;

/// @brief Holds and manages texture id.
/// @details Auto resizes an image if it not fit to
/// w == h == x^2 and saves uv coordinates
/// of image corners.
class GLTextureRes: public TextureRes {
public:
    GLTextureRes(SafePtr<flappy::Entity> rootEntity, std::shared_ptr<IRgbaBitmapRes> rgbaBitmapRes);
    ~GLTextureRes();
    void bind(GLShaderRes::UniformLocation uniformLoc, int n);
    std::list<std::shared_ptr<ResBase> > dependencyList();

private:
    GLuint m_texture = -1;

    SafePtr<Entity> m_rootEntity;
    std::shared_ptr<IRgbaBitmapRes> m_rgbaBitmapRes;

    void deinitGLTexture();
    void initGLTexture();
};

} // flappy
