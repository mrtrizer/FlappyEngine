#pragma once

#include <core/tools.h>

namespace flappy {

class ResourceMgr;
class Texture;
class Atlas;

template<typename ResourceT>
class ResourceHandler;

class Quad {
public:
    Quad(std::shared_ptr<ResourceHandler<Atlas>> atlas, const std::string& quadName, std::shared_ptr<ResourceMgr> resourceMgr):
        m_atlas(atlas),
        m_quadName(quadName),
        m_resourceMgr(resourceMgr)
    {}
    Tools::Rect rect();
    std::shared_ptr<ResourceHandler<Texture>> texture();

private:
    std::shared_ptr<ResourceHandler<Atlas>> m_atlas;
    std::string m_quadName;
    std::shared_ptr<ResourceMgr> m_resourceMgr;
};

} // flappy
