#pragma once

#include <Component.h>
#include <Color.h>
#include <TextureRes.h>
#include <RenderElementFactory.h>
#include <Entity.h>
#include <GlyphSheetRes.h>

namespace flappy {

class TextComponent: public Component<TextComponent> {
public:
    TextComponent()
    {
        addDependency(RenderElementFactory::id());

        subscribe([this](InitEvent) {
            m_renderElement = manager<RenderElementFactory>()->createTextRender(selfPointer());
            entity()->addComponent(m_renderElement);
        });

        subscribe([this](DeinitEvent) {
            entity()->removeComponent(m_renderElement);
            m_renderElement.reset();
        });
    }

    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color& colorRGBA() { return m_colorRGBA; }

    void setText(std::string text) { m_text = text; }
    std::string text() { return m_text; }

    void setTextureRes(std::shared_ptr<TextureRes> textureRes) { m_textureRes = textureRes; }
    std::shared_ptr<TextureRes> textureRes() {
        if (m_textureRes != nullptr && m_textureRes->resUpdated()) {
            m_textureRes = m_textureRes->lastRes();
        }
        return m_textureRes;
    }

    void setGlyphSheetRes(std::shared_ptr<GlyphSheetRes> glyphSheetRes) { m_glyphSheetRes = glyphSheetRes; }
    std::shared_ptr<GlyphSheetRes> glyphSheetRes() {
        if (m_glyphSheetRes != nullptr && m_glyphSheetRes->resUpdated()) {
            m_glyphSheetRes = m_glyphSheetRes->lastRes();
        }
        return m_glyphSheetRes;
    }

private:
    Color m_colorRGBA;
    std::string m_text;
    std::shared_ptr<TextureRes> m_textureRes;
    std::shared_ptr<GlyphSheetRes> m_glyphSheetRes;
    std::shared_ptr<View> m_renderElement;
};

} // flappy
