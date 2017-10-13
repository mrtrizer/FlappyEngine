#pragma once

#include <Component.h>
#include <Color.h>
#include <TextureRes.h>
#include <RenderElementFactory.h>
#include <Entity.h>
#include <GlyphSheetRes.h>
#include <FontRes.h>
#include <Tools.h>

namespace flappy {

class View;

class TextComponent: public Component<TextComponent> {
public:

    struct Box {
        Tools::Rect rect;
        GlyphSheetRes::Glyph glyph;
    };

    struct BoxedLexem {
        int width = 0;
        int height = 0;
        std::vector<Box> boxes;
    };

    struct BoxedLine {
        int width = 0;
        int height = 0;
        int yOffset = 0;
        std::vector<BoxedLexem> boxedLexems;
    };

    struct BoxedText {
        BoxedText() = default;
        int width = 0;
        int height = 0;
        std::vector<BoxedLine> boxedLines;
    };

    TextComponent();

    void setSize(int size) { m_size = size; }
    int size() { return m_size; }

    void setMaxWidth(int maxWidth) { m_maxWidth = maxWidth; }
    int maxWidth() { return m_maxWidth; }

    void setColorRGBA(Color colorRGBA) { m_colorRGBA = colorRGBA; }
    Color& colorRGBA() { return m_colorRGBA; }

    void setText(std::string text) { m_text = text; }
    std::string text() { return m_text; }

    void setFontRes(std::shared_ptr<FontRes> fontRes) { m_fontRes = fontRes; }
    std::shared_ptr<FontRes> fontRes();

    static glm::vec2 calcTextSize(std::string text, FontRes& fontRes, int width, int size);
    static BoxedLexem genBoxedLexem(std::string lexem, const GlyphSheetRes& glyphSheet, int size);
    static BoxedText genBoxedText(std::string text, const GlyphSheetRes& glyphSheet, int maxWidth, int size);
    static std::vector<std::string> splitIntoLexems(std::string str);

private:
    int m_size = 20;
    int m_maxWidth = 99999;
    Color m_colorRGBA;
    std::string m_text;
    BoxedText m_boxedText;
    std::shared_ptr<FontRes> m_fontRes;
    std::shared_ptr<View> m_renderElement;
};

} // flappy
