#include "screenmgr.h"

glm::vec2 ScreenMgr::screenSize() const {
    return glm::vec2(m_width, m_height);
}

void ScreenMgr::resize(int width, int height) {
    m_width = width;
    m_height = height;
}
