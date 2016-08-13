#pragma once

#include "glview.h"
#include "glshaderprogram.h"
#include "glattribarray.h"
#include "gltexture.h"
#include <core/resourcemgr.h>

namespace flappy {

class Sprite;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewSprite : public GLView<GLViewSprite>
{
public:
    GLViewSprite(const std::shared_ptr<ResourceHandler<Texture>> &glTexture, const Sprite &presenter);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    void update(const Presenter &) override;

private:
    GLAttribArray rect;
    std::shared_ptr<ResourceHandler<Texture>> texture;
    std::vector<GLTools::Vertex> vertexList;
    int lastFrameCount;
    int lastFrameN;

    void updateFrame(int frameCnt, int frameN);
    void updateVBOs();
};

} // flappy
