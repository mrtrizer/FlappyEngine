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

    struct TextChangedEvent : public IEvent {

    };

    enum class Align {
        LEFT,
        CENTER,
        RIGHT
    };

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

    void setSize(int size);
    int size() { return m_size; }

    void setAlign(Align align);
    Align align() { return m_align; }

    void setMaxWidth(int maxWidth);
    int maxWidth() { return m_maxWidth; }

    void setColor(Color color);
    Color& colorRGBA() { return m_color; }

    void setText(std::string text);
    std::string text() { return m_text; }

    void setFontRes(std::shared_ptr<FontRes> fontRes);
    std::shared_ptr<FontRes> fontRes();

    static glm::vec2 calcTextSize(std::string text, FontRes& fontRes, int width, int size);
    static BoxedLexem genBoxedLexem(std::string lexem, const GlyphSheetRes& glyphSheet, int size);
    static BoxedText genBoxedText(std::string text, const GlyphSheetRes& glyphSheet, int maxWidth, int size);
    static std::vector<std::string> splitIntoLexems(std::string str);
    static int calcLineOffset(Align align, BoxedText boxedText, BoxedLine boxedLine);

private:
    static constexpr int DEFAULT_WIDTH = 99999;
    int m_size = 20;
    Align m_align;
    int m_maxWidth = DEFAULT_WIDTH;
    Color m_color;
    std::string m_text;
    BoxedText m_boxedText;
    std::shared_ptr<FontRes> m_fontRes;
    std::shared_ptr<View> m_renderElement;
};

} // flappy
