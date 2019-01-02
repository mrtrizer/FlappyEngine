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
class GLTextRender : public GLRender
{
public:
    GLTextRender(Handle<Entity> entity);
    void setEntity(Handle<Entity> entity);
    void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) final;

private:
    GLAttribArray m_rect;
    Handle<TextComponent> m_textComponent;
    std::shared_ptr<FontRes> m_fontRes;
    bool m_textChanged = false;
    Handle<ResManager<ShaderRes>> m_shaderResManager;
    std::shared_ptr<ISubscription> m_subscription;

    void updateFrame();
};

} // flappy
