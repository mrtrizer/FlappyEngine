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

glm::vec2 TextComponent::calcTextSize(std::string text, FontRes& fontRes, int width) {
    auto lines = genBoxedText(text, *fontRes.glyphSheetRes(), width);
    return glm::vec2(lines.width, lines.height);
}

TextComponent::BoxedLexem TextComponent::genBoxedLexem(std::string lexem, const GlyphSheetRes &glyphSheet) {
    std::vector<Box> boxes;
    int offsetX = 0;
    for (char c : lexem) {
        Box box;
        auto glyph = glyphSheet.glyph((int)c);
        box.rect = Tools::Rect(glyph.xoffset + offsetX,
                                glyph.yoffset,
                                glyph.xoffset + offsetX + glyph.width,
                                glyph.yoffset + glyph.height);
        offsetX += glyph.xadvance;
        box.glyph = glyph;
        boxes.push_back(std::move(box));
    }
    BoxedLexem boxedLexem;
    boxedLexem.width = offsetX;
    boxedLexem.height = glyphSheet.common().base;
    boxedLexem.boxes = boxes;
    return boxedLexem;
}

TextComponent::BoxedText TextComponent::genBoxedText(std::string text, const GlyphSheetRes& glyphSheet, int maxWidth) {
    auto lexems = Tools::split(text, ' ');
    int yOffset = 0;
    int longestLineWidth = 0;
    BoxedText boxedText;
    LOGI("%d", boxedText.width);
    LOGI("%d", boxedText.height);
    boxedText.boxedLines.push_back(BoxedLine());
    auto currentLine = boxedText.boxedLines.begin();
    for (auto lexemString : lexems) {
        auto lexem = genBoxedLexem(lexemString, glyphSheet);
        if (lexem.width + currentLine->width > maxWidth) {
            yOffset += currentLine->height + glyphSheet.common().lineHeight;
            if (currentLine->width > longestLineWidth) {
                longestLineWidth = currentLine->width;
            }
            boxedText.boxedLines.push_back(BoxedLine());
            currentLine = boxedText.boxedLines.end() - 1;
            currentLine->yOffset = yOffset + glyphSheet.common().lineHeight;
        }
        currentLine->boxedLexems.push_back(lexem);
        currentLine->width += lexem.width;
    }
    boxedText.width = longestLineWidth;
    boxedText.height = yOffset + glyphSheet.common().lineHeight;
    return boxedText;
}

} // flappy
