#include "Component.h"

#include <EventController.h>
#include <Tools.h>

#include "Entity.h"
#include "Manager.h"

namespace flappy
{

ComponentBase::ComponentBase()
    : m_eventController(std::make_shared<EventController>())
{
    subscribeEvents();

    // Check if component initialized after the first update
    m_updateSubscription = subscribe([this](const UpdateEvent) {
        if (!isInitialized()) {
            for (auto dependency : m_dependenceComponentList) {
                if (!isComponentInitialized(dependency) && !isManagerInitialized(dependency)) {
                    LOGW("%s can't be initialized because dependency %s is not initialized.",
                         componentId().name().c_str(),
                         dependency.name().c_str());
                }
            }
        }
        events()->unsubscribe(m_updateSubscription);
    });
}

bool ComponentBase::isManagerInitialized(TypeId<ComponentBase> id) const {
    return m_managers.getById(id) != nullptr;
}

bool ComponentBase::isComponentInitialized(TypeId<flappy::ComponentBase> id) const {
    return m_components.getById(id) != nullptr;
}

bool ComponentBase::allDependenciesInitialized() const {
    for (TypeId<ComponentBase> dependenceTypeId : m_dependenceComponentList)
        if (!isComponentInitialized(dependenceTypeId) && !isManagerInitialized(dependenceTypeId))
            return false;
    return true;
}

void ComponentBase::subscribeEvents() {
    subscribe([this](const ManagerAddedEvent& e) {
        m_managers.setById(e.id, e.pointer);
        if (!isInitialized() && allDependenciesInitialized())
            tryInit();
    });
    subscribe([this](const ManagerRemovedEvent& e) {
        m_managers.setById(e.id, SafePtr<ManagerBase>());
        if (isInitialized() && !allDependenciesInitialized())
            tryDeinit();
    });
    subscribe([this](const ComponentAddedEvent& e) {
        if (m_components.getById(e.id) == nullptr)
            m_components.setById(e.id, e.pointer);
        if (!isInitialized() && allDependenciesInitialized())
            tryInit();
    });
    subscribe([this](const ComponentRemovedEvent& e) {
        m_components.setById(e.id, SafePtr<ComponentBase>());
        if (isInitialized() && !allDependenciesInitialized())
            tryDeinit();
    });
}

void ComponentBase::setParentEntity(SafePtr<Entity> entity)
{
    if (entity != m_entity) {
        // Deinit if entity was set before
        if (m_entity) {
            removedFromEntityInternal();
        }
        m_entity = entity;
        if (entity) {
            addedToEntityInternal();
        }
    }
}

void ComponentBase::addDependency(TypeId<ComponentBase> id) {
    m_dependenceComponentList.push_back(id);
}

void ComponentBase::tryInit() {
    // We have to set the flag before initialization call to avoid recursive calls
    m_initializedFlag = true;
    try {
        initInternal();
    } catch (std::exception& e) {
        LOGE("%s initialization error: %s", componentId().name().c_str(), e.what());
        m_initializedFlag = false;
    }
}

void ComponentBase::tryDeinit() {
    m_initializedFlag = false;
    try {
        // Send remove event first.
        // To allow components access manager before denitialization.
        deinitInternal();
    } catch (std::exception& e) {
        LOGE("%s deinitialization error: %s", componentId().name().c_str(), e.what());
        m_initializedFlag = true;
    }
}

void ComponentBase::addedToEntityInternal() {
    if (!isInitialized() && allDependenciesInitialized())
        tryInit();
    addedToEntity();
}

void ComponentBase::removedFromEntityInternal()
{
    if (isInitialized())
        tryDeinit();
    removedFromEntity();
}

} // flappy
