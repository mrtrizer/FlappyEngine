#include "resourcemgr.h"

namespace flappy {

template<> shared_ptr<Texture> ResourceMgr::get<Texture>(const string& path) const {
    return getTexture(path);
}

} // flappy
