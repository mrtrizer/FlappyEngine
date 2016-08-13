#include "resourcemgr.h"
#include "texture.h"
#include "atlas.h"

namespace flappy {

template<> std::unique_ptr<Texture> ResourceMgr::loadRes<Texture>(const std::string& path) const {
    return getTexture(path);
}

template<> std::unique_ptr<Atlas> ResourceMgr::loadRes<Atlas>(const std::string& path) const {
    return getAtlas(path);
}

} // flappy
