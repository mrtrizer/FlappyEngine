#pragma once

#include <unordered_map>

#include <Tools.h>
#include <Res.h>

namespace flappy {

class AtlasRes: public Res
{
public:
    struct SpriteInfo {
        SpriteInfo(Tools::Rect rectInAtlas, glm::vec2 size):
            rectInAtlas(rectInAtlas),
            size(size)
        {}

        /// Sprite position and size in atlas
        Tools::Rect rectInAtlas;
        /// Sprite size in logical coordinates
        glm::vec2 size;
    };

    AtlasRes() = default;
    SpriteInfo spriteInfo(const std::string& name) const;
    void addSpriteInfo(const std::string& name, const SpriteInfo& spriteInfo);
    std::list<std::shared_ptr<Res>> dependencyList() const final { return {}; }


private:  
    std::unordered_map<std::string, SpriteInfo> m_rectMap;
};

} // flappy
