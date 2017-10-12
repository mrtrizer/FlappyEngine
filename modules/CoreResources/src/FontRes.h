#pragma once

#include <glm/vec2.hpp>

#include <Tools.h>
#include <ResManager.h>

#include "GlyphSheetRes.h"
#include "TextureRes.h"

namespace flappy {

class FontRes: public Res<FontRes> {
public:
    FontRes(std::shared_ptr<GlyphSheetRes> glyphSheetRes, std::shared_ptr<TextureRes> textureRes):
        m_glyphSheetRes(glyphSheetRes),
        m_textureRes(textureRes)
    {}
    std::shared_ptr<GlyphSheetRes> glyphSheetRes() { return m_glyphSheetRes; }
    std::shared_ptr<TextureRes> textureRes() { return m_textureRes; }
    std::list<std::shared_ptr<ResBase>> dependencyList() const final { return {m_glyphSheetRes, m_textureRes}; }

private:
    std::shared_ptr<GlyphSheetRes> m_glyphSheetRes;
    std::shared_ptr<TextureRes> m_textureRes;
};

} // flappy
