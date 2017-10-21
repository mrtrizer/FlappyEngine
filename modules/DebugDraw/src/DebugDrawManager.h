#pragma once

#include <glm/glm.hpp>

#include <Manager.h>

#include "Tools.h"

namespace flappy {

class DebugDrawManager : public Manager<DebugDrawManager> {
public:
    DebugDrawManager();

    void drawRect(Tools::Rect rect, float liveTimeSec = 0.0f, std::string name = "");
    void drawCircle(glm::vec3 pos, int radius, float liveTimeSec = 0.0f, std::string name = "");
    void drawLine(glm::vec3 from, glm::vec3 to, float liveTimeSec = 0.0f, std::string name = "");
    void drawText(glm::vec3 pos, std::string text, float liveTimeSec = 0.0f, std::string name = "");
};

} // flappy
