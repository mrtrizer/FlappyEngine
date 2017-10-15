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
    auto component = m_components.getById(id);
    return (component != nullptr) && (component->isInitialized());
}

bool ComponentBase::allDependenciesInitialized() const {
    if (!isReady())
        return false;
    for (TypeId<ComponentBase> dependenceTypeId : m_dependenceComponentList)
        if (!isComponentInitialized(dependenceTypeId) && !isManagerInitialized(dependenceTypeId))
            return false;
    return true;
}

void ComponentBase::subscribeEvents() {
    subscribe([this](const ManagerAddedEvent& e) {
        m_managers.setById(e.id, e.pointer);
        if (!isInitialized() && allDependenciesInitialized() && active())
            tryInit();
    });
    subscribe([this](const ManagerRemovedEvent& e) {
        auto removedManager = m_managers.getById(e.id);
        // I remove manager from m_managers before calling of allDependenciesInitialized()
        m_managers.setById(e.id, SafePtr<ManagerBase>());
        if (isInitialized() && !allDependenciesInitialized()) {
            // Then I set it back to give oppotunity to use this manager during deinitialization
            m_managers.setById(e.id, removedManager);
            tryDeinit();
            // And then I totally remove it from the m_manager list
            m_managers.setById(e.id, SafePtr<ManagerBase>());
        }
    });
    subscribe([this](const ComponentAddedEvent& e) {
        if (m_components.getById(e.id) == nullptr)
            m_components.setById(e.id, e.pointer);
        if (!isInitialized() && allDependenciesInitialized() && active())
            tryInit();
    });
    subscribe([this](const ComponentRemovedEvent& e) {
        // See comments for ManagerRemovedEvent handler
        auto removedComponent = m_components.getById(e.id);
        m_components.setById(e.id, SafePtr<ComponentBase>());
        if (isInitialized() && !allDependenciesInitialized()) {
            m_components.setById(e.id, removedComponent);
            tryDeinit();
            m_components.setById(e.id, SafePtr<ManagerBase>());
        }
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
    m_initialization = true;
    try {
        initInternal();
    } catch (std::exception& e) {
        LOGE("%s initialization error: %s", componentId().name().c_str(), e.what());
        m_initializedFlag = false;
    }
    m_initialization = false;
}

void ComponentBase::tryDeinit() {
    m_initializedFlag = false;
    m_initialization = true;
    try {
        // Send remove event first.
        // To allow components access manager before denitialization.
        deinitInternal();
    } catch (std::exception& e) {
        LOGE("%s deinitialization error: %s", componentId().name().c_str(), e.what());
        m_initializedFlag = true;
    }
    m_initialization = false;
}

void ComponentBase::addedToEntityInternal() {
    if (!isInitialized() && allDependenciesInitialized() && active())
        tryInit();
    addedToEntity();
}

void ComponentBase::removedFromEntityInternal()
{
    if (isInitialized())
        tryDeinit();
    removedFromEntity();
}

void ComponentBase::unsubscribe(SafePtr<ISubscription> subscription) {
    events()->unsubscribe(subscription);
}

void ComponentBase::setActive(bool active) {
    m_active = active;
    if (m_initialization == false) {
        if (isInitialized() && !active) {
            tryDeinit();
        }
        if (!isInitialized() && allDependenciesInitialized() && active) {
            tryInit();
        }
    } else {
        throw std::runtime_error(Tools::format("Active state of %s is changed during initialization.",
                                               componentId().name().c_str()));
    }
}

} // flappy
