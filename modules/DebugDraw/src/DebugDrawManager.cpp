#include "DebugDrawManager.h"

#include <RenderElementFactory.h>
#include <RenderUtils.h>
#include <MeshComponent.h>
#include <TransformComponent.h>
#include <ResManager.h>
#include <MaterialRes.h>
#include <ShaderRes.h>
#include <IGLManager.h>
#include <RenderManager.h>
#include <DebugServices.h>

namespace flappy {

DebugDrawManager::DebugDrawManager(Handle<Hierarchy> hierarchy)
    : Updatable<DebugDrawManager>(hierarchy)
    , m_hierarchy(hierarchy)
{}

void DebugDrawManager::update(DeltaTime dt) {
    m_currentTime += dt;
    cleanUp();
}

void DebugDrawManager::cleanUp() {
    // Clean items with names
    for (auto pairIter = m_debugDrawItems.begin(); pairIter != m_debugDrawItems.end(); ) {
        if (pairIter->second.destroyTime <= m_currentTime) {
            pairIter = m_debugDrawItems.erase(pairIter);
        } else {
            ++pairIter;
        }
    }
    // Clean no name items
    m_noNameDebugDrawItems.remove_if([this](const auto& item) { return item.destroyTime <= m_currentTime; });
}

void DebugDrawManager::drawPolygon(std::vector<glm::vec3> vertices, float liveTimeSec, std::string name) {
    auto meshEntity = m_hierarchy->rootEntity()->createEntity();
    auto mesh = meshEntity->component<MeshComponent>();
    mesh->setVertices(vertices);
    auto material = m_hierarchy->manager<ResManager<MaterialRes>>()->getRes("wireframe_material", ExecType::ASYNC);
    mesh->setMaterialRes(material);

    if (name.empty())
        m_noNameDebugDrawItems.push_back({meshEntity, m_currentTime + liveTimeSec});
    else
        m_debugDrawItems.emplace(name, DebugDrawItem{meshEntity, liveTimeSec});
}

void DebugDrawManager::drawRect(MathUtils::Rect rect, float liveTimeSec, std::string name) {
    std::vector<glm::vec3> vertices = {
        {rect.x1, rect.y1, 0.0f},
        {rect.x2, rect.y1, 0.0f},
        {rect.x2, rect.y1, 0.0f},
        {rect.x2, rect.y2, 0.0f},
        {rect.x2, rect.y2, 0.0f},
        {rect.x1, rect.y2, 0.0f},
        {rect.x1, rect.y2, 0.0f},
        {rect.x1, rect.y1, 0.0f},
    };
    drawPolygon(vertices, liveTimeSec, name);
}

static std::vector<glm::vec3> genWireCircleVertices(float r, int vertexN, glm::vec3 pos)
{
    if (vertexN < 3)
        throw std::runtime_error("Too few vertices in circle (has to be >= 3).");
    std::vector<glm::vec3> vertexList(vertexN * 2);
    float step = M_PI * 2 / vertexN;
    for (int i = 0; i < vertexN; i++) {
        vertexList[i * 2 + 0] = pos + glm::vec3{cos(step * i) * r, sin(step * i) * r, 0.0f};
        int nextI = i + 1;
        vertexList[i * 2 + 1] = pos + glm::vec3{cos(step * nextI) * r, sin(step * nextI) * r, 0.0f};
    }
    return vertexList;
}

void DebugDrawManager::drawCircle(glm::vec3 pos, int radius, float liveTimeSec, std::string name) {
    drawPolygon(genWireCircleVertices(radius, 20, pos), liveTimeSec, name);
}

void DebugDrawManager::drawLine(glm::vec3 from, glm::vec3 to, float liveTimeSec, std::string name) {
    std::vector<glm::vec3> vertices = {
        from,
        to
    };
    drawPolygon(vertices, liveTimeSec, name);
}

void DebugDrawManager::drawText(glm::vec3, std::string, float, std::string) {
    LOGE("DebugDrawManager::drawText is not implemented yet");
}

} // flappy
