#include "DebugDrawManager.h"

#include <RenderElementFactory.h>
#include <RenderUtils.h>
#include <Tools.h>
#include <Entity.h>
#include <MeshComponent.h>
#include <TransformComponent.h>
#include <ResManager.h>
#include <MaterialRes.h>
#include <ShaderRes.h>
#include <IGLManager.h>
#include <RenderManager.h>

namespace flappy {

DebugDrawManager::DebugDrawManager()
{
    addDependency(RenderElementFactory::id());
    addDependency(ResManager<MaterialRes>::id());
    addDependency(ResManager<ShaderRes>::id());
    addDependency(IGLManager::id());

    subscribe([this](InitEvent) {
        for (auto pair : m_debugDrawItems) {
            pair.second.entity->events()->post(ManagerAddedEvent(manager<RenderElementFactory>()));
            pair.second.entity->events()->post(ManagerAddedEvent(manager<ResManager<MaterialRes>>()));
            pair.second.entity->events()->post(ManagerAddedEvent(manager<ResManager<ShaderRes>>()));
            pair.second.entity->events()->post(ManagerAddedEvent(manager<IGLManager>()));
            pair.second.entity->events()->post(ManagerAddedEvent(manager<RenderManager>()));
        }
        for (auto item : m_noNameDebugDrawItems){
            item.entity->events()->post(ManagerAddedEvent(manager<RenderElementFactory>()));
            item.entity->events()->post(ManagerAddedEvent(manager<ResManager<MaterialRes>>()));
            item.entity->events()->post(ManagerAddedEvent(manager<ResManager<ShaderRes>>()));
            item.entity->events()->post(ManagerAddedEvent(manager<IGLManager>()));
            item.entity->events()->post(ManagerAddedEvent(manager<RenderManager>()));
        }
    });

    subscribe([this](DeinitEvent) {
        for (auto pair : m_debugDrawItems) {
            pair.second.entity->events()->post(ManagerRemovedEvent(manager<RenderElementFactory>()));
            pair.second.entity->events()->post(ManagerRemovedEvent(manager<ResManager<MaterialRes>>()));
            pair.second.entity->events()->post(ManagerRemovedEvent(manager<ResManager<ShaderRes>>()));
            pair.second.entity->events()->post(ManagerRemovedEvent(manager<IGLManager>()));
            pair.second.entity->events()->post(ManagerRemovedEvent(manager<RenderManager>()));
        }
        for (auto item : m_noNameDebugDrawItems){
            item.entity->events()->post(ManagerRemovedEvent(manager<RenderElementFactory>()));
            item.entity->events()->post(ManagerRemovedEvent(manager<ResManager<MaterialRes>>()));
            item.entity->events()->post(ManagerRemovedEvent(manager<ResManager<ShaderRes>>()));
            item.entity->events()->post(ManagerRemovedEvent(manager<IGLManager>()));
            item.entity->events()->post(ManagerRemovedEvent(manager<RenderManager>()));
        }
    });

    subscribe([this](UpdateEvent e) {
        m_currentTime += e.dt;
        cleanUp();
    });
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

void DebugDrawManager::drawRect(Tools::Rect rect, float liveTimeSec, std::string name) {

}

void DebugDrawManager::drawCircle(glm::vec3 pos, int radius, float liveTimeSec, std::string name) {
    auto meshEntity = std::make_shared<Entity>();
    meshEntity->component<MeshComponent>()->setVertices(genCircleVertices(radius, 20));
    if (isInitialized()) {
        meshEntity->events()->post(ManagerAddedEvent(manager<RenderElementFactory>()));
        meshEntity->events()->post(ManagerAddedEvent(manager<ResManager<MaterialRes>>()));
        meshEntity->events()->post(ManagerAddedEvent(manager<ResManager<ShaderRes>>()));
        meshEntity->events()->post(ManagerAddedEvent(manager<IGLManager>()));
        meshEntity->events()->post(ManagerAddedEvent(manager<RenderManager>()));
    }
    meshEntity->component<TransformComponent>()->setPos(pos);
    if (name.empty())
        m_noNameDebugDrawItems.push_back({meshEntity, m_currentTime + liveTimeSec});
    else
        m_debugDrawItems.emplace(name, DebugDrawItem{meshEntity, liveTimeSec});
}

void DebugDrawManager::drawLine(glm::vec3 from, glm::vec3 to, float liveTimeSec, std::string name) {

}

void DebugDrawManager::drawText(glm::vec3 pos, std::string text, float liveTimeSec, std::string name) {

}

} // flappy
