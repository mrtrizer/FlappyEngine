#pragma once

#include <Color.h>

#warning FIXME: Strange includes
#include <GLView.h>
#include <GLAttribArray.h>

namespace flappy {

template <typename T>
class ResHandler;

class QuadRes;

class SpriteComponent;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewSprite : public GLView<GLViewSprite>
{
public:
    GLViewSprite();
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;
    void update(const PresenterComponent &) override;

private:
    GLAttribArray m_rect;

    GLTools::GLColorRGBA m_colorRGBA;

    std::vector<GLTools::Vertex> m_vertexList;

    std::shared_ptr<QuadRes> m_quad;

    void updateFrame();
};

} // flappy
