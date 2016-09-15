#pragma once

#include <core/color.h>

#include "glview.h"
#include "glattribarray.h"

namespace flappy {

template <typename T>
class ResourceHandler;

class Quad;

class Sprite;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewSprite : public GLView<GLViewSprite>
{
public:
    GLViewSprite();
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    void update(const Presenter &) override;

private:
    GLAttribArray m_rect;

    GLTools::GLColorRGBA m_colorRGBA;

    std::vector<GLTools::Vertex> m_vertexList;

    std::shared_ptr<ResourceHandler<Quad>> m_quad;

    void updateFrame();
    void updateVBOs();
};

} // flappy
