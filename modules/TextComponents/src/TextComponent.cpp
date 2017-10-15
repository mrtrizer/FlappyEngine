#include "TextComponent.h"
#include <utfcpp/utf8.h>

#include <View.h>

namespace flappy {

TextComponent::TextComponent()
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

void TextComponent::setSize(int size) {
    m_size = size;
    entity()->events()->post(TextChangedEvent());
}

void TextComponent::setAlign(Align align) {
    m_align = align;
    entity()->events()->post(TextChangedEvent());
}

void TextComponent::setMaxWidth(int maxWidth) {
    m_maxWidth = maxWidth;
    entity()->events()->post(TextChangedEvent());
}

void TextComponent::setColor(Color color) {
    m_color = color;
    entity()->events()->post(TextChangedEvent());
}

void TextComponent::setText(std::string text) {
    m_text = text;
    entity()->events()->post(TextChangedEvent());
}

void TextComponent::setFontRes(std::shared_ptr<FontRes> fontRes) {
    m_fontRes = fontRes;
    entity()->events()->post(TextChangedEvent());
}

std::shared_ptr<FontRes> TextComponent::fontRes() {
    if (m_fontRes != nullptr && m_fontRes->resUpdated())
        m_fontRes = m_fontRes->lastRes();
    return m_fontRes;
}

glm::vec2 TextComponent::calcTextSize(std::string text, FontRes& fontRes, int width, int size) {
    auto lines = genBoxedText(text, *fontRes.glyphSheetRes(), width, size);
    return glm::vec2(lines.width, lines.height);
}

TextComponent::BoxedLexem TextComponent::genBoxedLexem(std::string lexemStr, const GlyphSheetRes &glyphSheet, int size) {
    if (lexemStr == "\n")
        return BoxedLexem();
    std::u32string utf32LexemStr;
    utf8::utf8to32(lexemStr.begin(), lexemStr.end(), std::back_inserter(utf32LexemStr));
    std::vector<Box> boxes;
    int offsetX = 0;
    for (int i = 0; i < utf32LexemStr.size(); i++) {
        char32_t code = utf32LexemStr[i];
        auto glyph = glyphSheet.glyph(code);
        const int base = glyphSheet.common().base;
        const int newXOffset = (glyph.xoffset * size) / base;
        const int newYOffset = (glyph.yoffset * size) / base;
        const int newWidth = (glyph.width * size) / base;
        const int newHeight = (glyph.height * size) / base;
        int prevCode = (i != 0) ?utf32LexemStr[i - 1]: 0;
        // Apply kerning if the symbol is not the first in a lexem
        auto kerning = glyphSheet.kerning(glyph.id,  prevCode);
        const float kerningAmount = (kerning.amount * size) / base;
        Box box;
        box.rect = Tools::Rect(newXOffset + offsetX + kerningAmount,
                                newYOffset,
                                newXOffset + offsetX + newWidth + kerningAmount,
                                newYOffset + newHeight);
        box.glyph = glyph;
        boxes.push_back(std::move(box));
        const int newXAdvance = (glyph.xadvance * size) / base;
        offsetX += newXAdvance;
    }
    BoxedLexem boxedLexem;
    boxedLexem.width = offsetX;
    boxedLexem.height = size;
    boxedLexem.boxes = boxes;
    return boxedLexem;
}

TextComponent::BoxedText TextComponent::genBoxedText(std::string text, const GlyphSheetRes& glyphSheet, int maxWidth, int size) {
    auto lexems = splitIntoLexems(text);
    int yOffset = 0;
    int longestLineWidth = maxWidth < DEFAULT_WIDTH ? maxWidth: 0;
    const int base = glyphSheet.common().base;
    const int lineOffset = ((glyphSheet.common().lineHeight - base) * size) / base;
    BoxedText boxedText;
    boxedText.boxedLines.push_back(BoxedLine());
    auto currentLine = boxedText.boxedLines.begin();
    for (auto lexem : lexems) {
        auto boxedLexem = genBoxedLexem(lexem, glyphSheet, size);
        if (boxedLexem.width + currentLine->width > maxWidth || lexem == "\n") {
            yOffset += currentLine->height + lineOffset;
            longestLineWidth = std::max(currentLine->width, longestLineWidth);
            boxedText.boxedLines.push_back(BoxedLine());
            currentLine = boxedText.boxedLines.end() - 1;
            currentLine->yOffset = yOffset;
        }
        if (currentLine->width == 0 && lexem == " ") {
            // skip a space in the begining of a line
        } else {
            currentLine->boxedLexems.push_back(boxedLexem);
            currentLine->width += boxedLexem.width;
            currentLine->height = std::max(boxedLexem.height, currentLine->height);
        }
    }
    boxedText.width = longestLineWidth;
    boxedText.height = yOffset + lineOffset;
    return boxedText;
}

std::vector<std::string> TextComponent::splitIntoLexems(std::string str) {
    std::vector<std::string> lexems;
    int startPos = 0;
    for (int i = 0; i < str.length(); i++) {
        char c = str[i];
        if ((c == ' ') || (c == '\n')) {
            if (startPos != i)
                lexems.push_back(str.substr(startPos, i - startPos));
            lexems.push_back(std::string(1, c));
            startPos = i + 1;
        }
    }
    lexems.push_back(str.substr(startPos, str.length() - startPos));
    return lexems;
}

int TextComponent::calcLineOffset(Align align, BoxedText boxedText, BoxedLine boxedLine) {
    if (align == Align::CENTER)
        return (boxedText.width - boxedLine.width) / 2;
    if (align == Align::RIGHT)
        return boxedText.width - boxedLine.width;
    return 0;
}

} // flappy
