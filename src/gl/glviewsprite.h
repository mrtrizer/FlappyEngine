#pragma once

#include "glview.h"
#include "glshaderprogram.h"
#include "glattribarray.h"
#include "gltexture.h"

namespace flappy {

using namespace std;
using namespace glm;

class Sprite;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewSprite : public GLView<GLViewSprite>
{
public:
    GLViewSprite(const shared_ptr<Texture> &glTexture, const Sprite &presenter);
    void draw(const mat4 & pMartrix, const mat4 & mvMatrix) override;
    void update(const Presenter &) override;
private:
    GLAttribArray rect;
    shared_ptr<GLTexture> texture;
    vector<GLTools::Vertex> vertexList;
};

} // flappy
