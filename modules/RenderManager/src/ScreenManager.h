#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "CameraComponent.h"

namespace flappy {

class [[manager]] ScreenManager
{
public:
    const glm::uvec2& screenSize() const { return m_screenSize; }
    void resize(const glm::uvec2& size);
    
private:
    glm::uvec2 m_screenSize;
};

} // flappy
