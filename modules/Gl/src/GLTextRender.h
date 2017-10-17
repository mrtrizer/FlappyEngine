#pragma once

#include <Color.h>

#include <GLRender.h>
#include <GLAttribArray.h>

namespace flappy {

template <typename T>
class ResHandler;

class FontRes;
class TextComponent;

/// Contains shaders and vbos to draw textures on a rectangle.
class GLTextRender : public GLRender<GLTextRender>
{
public:
    GLTextRender(SafePtr<TextComponent> textComponent);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) final;

private:
    GLAttribArray m_rect;
    SafePtr<TextComponent> m_textComponent;
    std::shared_ptr<FontRes> m_fontRes;
    bool m_textChanged = false;

    void updateFrame();
};

} // flappy
