#include "DebugDrawManager.h"

#include <RenderManager.h>
#include <Tools.h>

namespace flappy {

DebugDrawManager::DebugDrawManager()
{
    addDependency(RenderManager::id());

    subscribe([this](UpdateEvent e) {

    });
}

void DebugDrawManager::drawRect(Tools::Rect rect, float liveTimeSec, std::string name) {

}

void DebugDrawManager::drawCircle(glm::vec3 pos, int radius, float liveTimeSec, std::string name) {

}

void DebugDrawManager::drawLine(glm::vec3 from, glm::vec3 to, float liveTimeSec, std::string name) {

}

void DebugDrawManager::drawText(glm::vec3 pos, std::string text, float liveTimeSec, std::string name) {

}

} // flappy
