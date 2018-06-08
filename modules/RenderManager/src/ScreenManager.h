#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <Manager.h>

#include "CameraComponent.h"

namespace flappy {

class ScreenManager: public Manager<ScreenManager>
{
public:
    ScreenManager(int width, int height);
    glm::vec2 screenSize() const;
    void resize(int width, int height);

    int width() const {return m_width;}
    int height() const {return m_height;}

private:
    int m_width = 1;
    int m_height = 1;
};

} // flappy
