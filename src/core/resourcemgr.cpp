#include "resourcemgr.h"

template<> std::shared_ptr<Texture> ResourceMgr::get<Texture>(const std::string& path) const {
    return getTexture(path);
}
