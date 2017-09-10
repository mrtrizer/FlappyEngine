#pragma once

#include <glm/vec2.hpp>

#include <Tools.h>
#include <ResManager.h>

#include "AtlasRes.h"
#include "TextureRes.h"

namespace flappy {

class SpriteRes: public Res {
public:
    SpriteRes(std::shared_ptr<AtlasRes> atlasRes, std::shared_ptr<TextureRes> textureRes, const std::string& quadName):
        m_atlasRes(atlasRes),
        m_textureRes(textureRes),
        m_quadName(quadName)
    {}
    AtlasRes::SpriteInfo spriteInfo() { return m_atlasRes->spriteInfo(m_quadName); }
    std::shared_ptr<TextureRes> texture() { return m_textureRes; }
    std::list<std::shared_ptr<Res>> dependencyList() const override { return {m_atlasRes, m_textureRes}; }

private:
    std::shared_ptr<AtlasRes> m_atlasRes;
    std::shared_ptr<TextureRes> m_textureRes;
    std::string m_quadName;
};

} // flappy
