#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "manager.h"
#include "flappyapp.h"

namespace flappy {

using namespace glm;

class ScreenMgr: public Manager<ScreenMgr>
{
public:
    vec2 screenSize() const;
    void resize(int width, int height);

    int width() const {return m_width;}
    int height() const {return m_height;}

private:
    int m_width = 1;
    int m_height = 1;
    float m_coefff = 1.0f;
};

namespace Screen
{
    inline vec2 screenSize() {return FlappyApp::inst().screenMgr()->screenSize();}
}

} // flappy
