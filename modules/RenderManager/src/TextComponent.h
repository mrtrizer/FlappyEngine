#pragma once

#include <Component.h>
#include <Color.h>
#include <TextureRes.h>
#include <RenderElementFactory.h>
#include <Entity.h>
#include <GlyphSheetRes.h>
#include <FontRes.h>

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

    void setSize(int size) { m_size = size; }
    int size() { return m_size; }

    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color& colorRGBA() { return m_colorRGBA; }

    void setText(std::string text) { m_text = text; }
    std::string text() { return m_text; }

    void setFontRes(std::shared_ptr<FontRes> fontRes) { m_fontRes = fontRes; }
    std::shared_ptr<FontRes> fontRes() {
        if (m_fontRes != nullptr && m_fontRes->resUpdated())
            m_fontRes = m_fontRes->lastRes();
        return m_fontRes;
    }

private:
    int m_size = 20;
    Color m_colorRGBA;
    std::string m_text;
    std::shared_ptr<FontRes> m_fontRes;
    std::shared_ptr<View> m_renderElement;
};

} // flappy
