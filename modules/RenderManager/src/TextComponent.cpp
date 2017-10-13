#include "TextComponent.h"

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

std::shared_ptr<FontRes> TextComponent::fontRes() {
    if (m_fontRes != nullptr && m_fontRes->resUpdated())
        m_fontRes = m_fontRes->lastRes();
    return m_fontRes;
}

glm::vec2 TextComponent::calcTextSize(std::string text, FontRes& fontRes, int width, int size) {
    auto lines = genBoxedText(text, *fontRes.glyphSheetRes(), width, size);
    return glm::vec2(lines.width, lines.height);
}

TextComponent::BoxedLexem TextComponent::genBoxedLexem(std::string lexem, const GlyphSheetRes &glyphSheet, int size) {
    std::vector<Box> boxes;
    int offsetX = 0;
    for (char c : lexem) {
        Box box;
        auto glyph = glyphSheet.glyph((int)c);
        const int base = glyphSheet.common().base;
        const int newXOffset = (glyph.xoffset * size) / base;
        const int newYOffset = (glyph.yoffset * size) / base;
        const int newWidth = (glyph.width * size) / base;
        const int newHeight = (glyph.height * size) / base;
        box.rect = Tools::Rect(newXOffset + offsetX,
                                newYOffset,
                                newXOffset + offsetX + newWidth,
                                newYOffset + newHeight);
        const int newXAdvance = (glyph.xadvance * size) / base;
        offsetX += newXAdvance;
        box.glyph = glyph;
        boxes.push_back(std::move(box));
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
    int longestLineWidth = 0;
    const int base = glyphSheet.common().base;
    const int newLineHeight = (glyphSheet.common().lineHeight * size) / base;
    BoxedText boxedText;
    boxedText.boxedLines.push_back(BoxedLine());
    auto currentLine = boxedText.boxedLines.begin();
    for (auto lexem : lexems) {
        auto boxedLexem = genBoxedLexem(lexem, glyphSheet, size);
        if (boxedLexem.width + currentLine->width > maxWidth || lexem == "\n") {
            yOffset += currentLine->height + newLineHeight;
            if (currentLine->width > longestLineWidth) {
                longestLineWidth = currentLine->width;
            }
            boxedText.boxedLines.push_back(BoxedLine());
            currentLine = boxedText.boxedLines.end() - 1;
            currentLine->yOffset = yOffset;
        }
        currentLine->boxedLexems.push_back(boxedLexem);
        currentLine->width += boxedLexem.width;
        if (boxedLexem.height > currentLine->height)
            currentLine->height = boxedLexem.height;
    }
    boxedText.width = longestLineWidth;
    boxedText.height = yOffset + newLineHeight;
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

} // flappy
