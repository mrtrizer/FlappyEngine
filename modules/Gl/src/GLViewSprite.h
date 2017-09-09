#pragma once

#include <Color.h>

// FIXME: Strange includes
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
    GLViewSprite(SafePtr<SpriteComponent> spriteComponent);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) override;

private:
    GLAttribArray m_rect;
    SafePtr<SpriteComponent> m_spriteComponent;
    std::vector<GLTools::Vertex> m_vertexList;
    std::shared_ptr<QuadRes> m_quadRes;
    std::shared_ptr<GLShaderProgram> m_shaderProgramRes;

    void updateFrame();
};

} // flappy
