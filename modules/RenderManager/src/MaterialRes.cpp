#include "MaterialRes.h"

#include <DebugServices.h>

namespace flappy {

void MaterialRes::setRenderModeFromStr(std::string mode) {
    try {
        m_renderMode = m_renderModes[mode];
    } catch (const std::exception&) {
        LOGE("Unknown render mode \"%s\"", mode.c_str());
    }
}

} // flappy
