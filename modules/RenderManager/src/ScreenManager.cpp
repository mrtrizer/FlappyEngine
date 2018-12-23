#include "ScreenManager.h"

namespace flappy {

using namespace glm;

ScreenManager::ScreenManager(int width, int height)
    : m_width(width)
    , m_height(height)
{}

vec2 ScreenManager::screenSize() const {
    return vec2(m_width, m_height);
}

void ScreenManager::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw std::runtime_error("Invalid screen size. It should be greater then 0.");
    m_width = width;
    m_height = height;
}

} // flappy
