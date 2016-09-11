#include "screenmanager.h"

namespace flappy {

using namespace glm;

vec2 ScreenManager::screenSize() const {
    return vec2(m_width, m_height);
}

void ScreenManager::resize(int width, int height) {
    m_width = width;
    m_height = height;
}

} // flappy
