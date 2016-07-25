#ifndef GVIEWSPRITE_H
#define GVIEWSPRITE_H

#include "glview.h"
#include "glshaderprogram.h"
#include "glattribarray.h"
#include "gltexture.h"

class Sprite;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewSprite : public GLView<GLViewSprite>
{
public:
    GLViewSprite(const std::shared_ptr<Texture> &glTexture, const Sprite &presenter);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    void update(const Presenter &) override;
private:
    GLAttribArray rect;
    std::shared_ptr<GLTexture> texture;
    std::vector<GLTools::Vertex> vertexList;
};

#endif // GVIEWSPRITE_H
