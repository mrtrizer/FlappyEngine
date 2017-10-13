#include "GlyphSheetRes.h"

namespace flappy {

using namespace std;
using namespace Tools;

GlyphSheetRes::GlyphSheetRes(std::shared_ptr<JsonRes> jsonRes)
    : m_jsonRes(jsonRes)
{}

void GlyphSheetRes::addGlyph(char32_t id, Glyph glyph) {
    m_glyphMap.emplace(id, glyph);
}

GlyphSheetRes::Glyph GlyphSheetRes::glyph(char32_t id) const {
    auto rectIter = m_glyphMap.find(id);
    if (rectIter != m_glyphMap.end())
        return rectIter->second;
    LOGE("Glyph %d is not found in the sheet.", id);
    return m_glyphMap.begin()->second;
}

void GlyphSheetRes::addKerning(Kerning kerning) {
    m_kerningList.push_back(kerning);
}

GlyphSheetRes::Kerning GlyphSheetRes::kerning(int firstId, int secondId) const {
    for (auto kerning : m_kerningList) {
        if (kerning.first == firstId && kerning.second == secondId)
            return kerning;
    }
    Kerning defaultKerning;
    defaultKerning.first = firstId;
    defaultKerning.second = secondId;
    defaultKerning.amount = 0;
    return defaultKerning;
}

} // flappy

