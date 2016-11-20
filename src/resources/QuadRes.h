#pragma once

#include <glm/vec2.hpp>

#include <core/tools.h>
#include <managers/ResManager/Res.h>

#include "AtlasRes.h"
#include "TextureRes.h"

namespace flappy {

class QuadRes: public Res {
public:
    QuadRes(std::shared_ptr<AtlasRes> atlasRes, std::shared_ptr<TextureRes> textureRes, const std::string& quadName):
        m_atlasRes(atlasRes),
        m_textureRes(textureRes),
        m_quadName(quadName)
    {}
    Tools::Rect rect() { return m_atlasRes->rect(m_quadName); }
    std::shared_ptr<TextureRes> texture() { return m_textureRes; }
    std::list<std::shared_ptr<Res>> dependencyList() const override { return {m_atlasRes, m_textureRes}; }

private:
    std::shared_ptr<AtlasRes> m_atlasRes;
    std::shared_ptr<TextureRes> m_textureRes;
    std::string m_quadName;
};

} // flappy
