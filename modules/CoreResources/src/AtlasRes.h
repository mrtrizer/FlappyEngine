#pragma once

#include <unordered_map>

#include <MathUtils.h>
#include <Res.h>
#include <JsonRes.h>

namespace flappy {

class AtlasRes: public Res<AtlasRes>
{
public:
    struct SpriteInfo {
        SpriteInfo(const MathUtils::Rect& rectInAtlas, glm::vec2 size):
            rectInAtlas(rectInAtlas),
            size(size)
        {}

        // TODO: Rename to rect
        /// Sprite position and size in atlas
        MathUtils::Rect rectInAtlas;
        /// Sprite size in logical coordinates
        glm::vec2 size;
    };

    AtlasRes(std::shared_ptr<JsonRes> jsonRes = nullptr);
    SpriteInfo spriteInfo(const std::string& name) const;
    void addSpriteInfo(const std::string& name, const SpriteInfo& spriteInfo);
    std::list<std::shared_ptr<ResBase>> dependencyList() const final { return {m_jsonRes}; }


private:  
    std::unordered_map<std::string, SpriteInfo> m_rectMap;
    std::shared_ptr<JsonRes> m_jsonRes;
};

} // flappy
