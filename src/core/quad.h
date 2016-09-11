#pragma once

#include <glm/vec2.hpp>

#include <core/tools.h>

namespace flappy {

class ResManager;
class Texture;
class Atlas;

template<typename ResourceT>
class ResourceHandler;

class Quad {
public:
    Quad(std::shared_ptr<ResourceHandler<Atlas>> atlas, const std::string& quadName, std::shared_ptr<ResManager> resourceManager):
        m_atlas(atlas),
        m_quadName(quadName),
        m_resourceManager(resourceManager)
    {}
    Tools::Rect rect();
    std::shared_ptr<ResourceHandler<Texture>> texture();

private:
    std::shared_ptr<ResourceHandler<Atlas>> m_atlas;
    std::string m_quadName;
    std::shared_ptr<ResManager> m_resourceManager;
};

} // flappy
