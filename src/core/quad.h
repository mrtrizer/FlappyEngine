#pragma once

#include <glm/vec2.hpp>

#include <core/tools.h>

namespace flappy {

class ResManager;
class Texture;
class Atlas;

template<typename ResourceT>
class ResHandler;

class Quad {
public:
    Quad(std::shared_ptr<ResHandler<Atlas>> atlas, const std::string& quadName, std::shared_ptr<ResManager> resManager):
        m_atlas(atlas),
        m_quadName(quadName),
        m_resManager(resManager)
    {}
    Tools::Rect rect();
    std::shared_ptr<ResHandler<Texture>> texture();

private:
    std::shared_ptr<ResHandler<Atlas>> m_atlas;
    std::string m_quadName;
    std::weak_ptr<ResManager> m_resManager;
};

} // flappy
