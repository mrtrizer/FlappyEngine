#include "AtlasRes.h"

#include <DebugServices.h>

namespace flappy {

using namespace std;

AtlasRes::AtlasRes(std::shared_ptr<JsonRes> jsonRes)
    : m_jsonRes(jsonRes)
{}

AtlasRes::SpriteInfo AtlasRes::spriteInfo(const string& name) const {
    auto rectIter = m_rectMap.find(name);
    if (rectIter != m_rectMap.end())
        return rectIter->second;
    LOGE("Rect with name %s is not found in the atlas.", name.data());
    return AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,1.0f}, {0.0f, 0.0f});
}

void AtlasRes::addSpriteInfo(const string& name, const SpriteInfo& spriteInfo) {
    m_rectMap.emplace(name, spriteInfo);
}

} // flappy

