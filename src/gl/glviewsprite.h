#ifndef GVIEWSPRITE_H
#define GVIEWSPRITE_H

#include "glview.h"
#include "glshaderprogram.h"
#include "glattribarray.h"
#include "gltexture.h"

class GPresenterSprite;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewSprite : public GLView<GLViewSprite>
{
public:
    GLViewSprite(const std::shared_ptr<GLTexture> &glTexture, const GPresenterSprite &presenter);
    void draw(const PMatrix, const MVMatrix) override;
    void update(const GPresenter &) override;
private:
    GLAttribArray rect;
    std::shared_ptr<GLTexture> texture;
    std::vector<GLTools::Vertex> vertexList;
};

#endif // GVIEWSPRITE_H
