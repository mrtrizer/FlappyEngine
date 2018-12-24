#include "ScreenManager.h"

namespace flappy {

void ScreenManager::resize(const glm::uvec2& size) {
    m_screenSize = size;
    m_eventBus.post(ScreenResizedEvent());
}

} // flappy
