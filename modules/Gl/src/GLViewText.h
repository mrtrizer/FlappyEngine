#pragma once

#include <Color.h>

#include <GLView.h>
#include <GLAttribArray.h>

namespace flappy {

template <typename T>
class ResHandler;

class TextureRes;
class GlyphSheetRes;
class TextComponent;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLViewText : public GLView<GLViewText>
{
public:
    GLViewText(SafePtr<TextComponent> spriteComponent);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) final;

private:
    GLAttribArray m_rect;
    SafePtr<TextComponent> m_spriteComponent;
    std::shared_ptr<TextureRes> m_textureRes;
    std::shared_ptr<GlyphSheetRes> m_glyphSheetRes;

    void updateFrame();
};

} // flappy
