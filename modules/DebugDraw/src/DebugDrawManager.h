#pragma once

#include <unordered_map>
#include <memory>

#include <glm/glm.hpp>

#include <Handle.hpp>
#include <MathUtils.h>
#include <Updatable.hpp>

namespace flappy {

class Hierarchy;
class Entity;

class REFLECT DebugDrawManager : public Updatable<DebugDrawManager> {
public:
    DebugDrawManager(Handle<Hierarchy> hierarchy);
    void update(DeltaTime dt);

    void drawPolygon(std::vector<glm::vec3>, float liveTimeSec = 0.0f, std::string name = "");
    void drawRect(MathUtils::Rect rect, float liveTimeSec = 0.0f, std::string name = "");
    void drawCircle(glm::vec3 pos, int radius, float liveTimeSec = 0.0f, std::string name = "");
    void drawLine(glm::vec3 from, glm::vec3 to, float liveTimeSec = 0.0f, std::string name = "");
    void drawText(glm::vec3, std::string, float = 0.0f, std::string = "");

private:
    struct DebugDrawItem {
        Handle<Entity> entity;
        float destroyTime = 0.0f;
    };

    Handle<Hierarchy> m_hierarchy;
    float m_currentTime = 0.0f;
    std::unordered_map<std::string, DebugDrawItem> m_debugDrawItems;
    std::list<DebugDrawItem> m_noNameDebugDrawItems;

    void cleanUp();
};

} // flappy
