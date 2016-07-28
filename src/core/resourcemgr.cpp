#include "resourcemgr.h"

template<> std::shared_ptr<Texture> ResourceMgr::get<Texture>(std::string path) const {
    return getTexture(path);
}
