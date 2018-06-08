#pragma once

#include <memory>

// To not include everywhere
#include <EventController.h>
#include <SafePtr.h>
#include <Tools.h>

namespace flappy
{

class ManagerBase;
template <typename DerivedT>
class Manager;
class Entity;
class EventController;

/// Base class for all components. Components can be attached to the
/// entity and have access to neighbour components to the same entity
/// via methods of entity.
/// @details Component can be in two states:
/// - Initialized - When attached to an entity and all dependencies initialized.
/// - Deinitialized - When detached of an entity or dependencies are not initialized.
/// Once component switch state to initialized, InitEvent is sent into event bus of
/// this  component. And ComponentAdded event is sent to event bus of entity where
/// component is attached to.
/// Other components can understand that dependancy is initialized/deinitialized
/// by listening of ComponentAddedEvent and ComponentRemovedEvent events.
/// Components can also depend of and access to a special type of components called
/// Managers if it placed higher or on the same level in the hierarchy.
class ComponentBase: public std::enable_shared_from_this<ComponentBase>
{
    friend class Entity;
public:

    struct UpdateEvent: public IEvent
    {
        UpdateEvent(DeltaTime dt)
            : dt(dt)
        {}
        DeltaTime dt;
    };

    struct InitEvent: public IEvent
    {};

    struct DeinitEvent: public IEvent
    {};

    struct ComponentEvent: public IEvent
    {
        ComponentEvent(SafePtr<ComponentBase> pointer)
            : id (pointer->componentId())
            , pointer(pointer)
        {}
        ComponentEvent()
        {}
        TypeId<ComponentBase> id;
        SafePtr<ComponentBase> pointer;
        template <typename ComponentT>
        SafePtr<ComponentT> castTo() {
            if (GetTypeId<ComponentBase, ComponentT>::value() == id)
                return SafePtr<ComponentT>(pointer);
            else
                return SafePtr<ComponentT>();
        }
    };

    struct ManagerAddedEvent: public ComponentEvent
    { using ComponentEvent::ComponentEvent; };

    struct ManagerRemovedEvent: public ComponentEvent
    { using ComponentEvent::ComponentEvent; };

    struct ComponentAddedEvent: public ComponentEvent
    { using ComponentEvent::ComponentEvent; };

    struct ComponentRemovedEvent: public ComponentEvent
    { using ComponentEvent::ComponentEvent; };

    ComponentBase();
    ComponentBase(const ComponentBase&) = delete;
    ComponentBase& operator=(const ComponentBase&) = delete;
    ComponentBase(ComponentBase&&) = default;
    ComponentBase& operator=(ComponentBase&&) & = default;
    virtual ~ComponentBase() = default;

    virtual TypeId<ComponentBase> componentId() const = 0;

    SafePtr<Entity> entityPtr() const { return m_entitySafePtr; }

    /// Returns EventController
    std::shared_ptr<EventController> events() { return m_eventController; }

    bool isInitialized() { return m_initializedFlag; }

    /// Override this method to define special conditions of component initialization
    virtual bool isReady() const { return true; }

    /// Allows you activate or deactivate component. Components are activated by default.
    /// Deactivated component can't be initialized.
    void setActive(bool active);

    /// Returns value of active field. Components are activated by default.
    bool active() { return m_active; }

    TypeMap<ComponentBase, SafePtr<ManagerBase>> managers() {
        return m_managers;
    }

protected:
    /// Returns parent entity (can be null if conponent is not added to entity)
    Entity* entity() const { return m_entity; }

    bool isManagerInitialized(TypeId<ComponentBase> id) const;

    bool isComponentInitialized(TypeId<ComponentBase> id) const;

    /// Returns true of all dependencies are initialized.
    bool allDependenciesInitialized() const;

    /// Returns true if component is listed in dependencies of the component.
    template <typename ComponentT>
    bool isDependancy() const;

    /// Returns manager if avaliable
    /// You can access only managers at the same or higer level of hierarchy.
    template <typename ManagerT>
    SafePtr<ManagerT> manager() const;

    /// Use this method to list dependencies.
    /// Component will be initialized once all depenencies are initialized.
    void addDependency(TypeId<ComponentBase> id);

    /// Short method for calling of events()->subscribe(...)
    template <typename FuncT>
    SafePtr<ISubscription> subscribe(FuncT&& func);

    /// Short method for calling of events()->subscribe(...)
    void unsubscribe(SafePtr<ISubscription> subscription);

private:
    using TypeIdList = std::list<TypeId<ComponentBase>>;

    bool m_initializedFlag = false;
    bool m_initialization = false;
    bool m_active = true;
    bool m_firstUpdate = true;
    Entity* m_entity = nullptr;
    SafePtr<Entity> m_entitySafePtr;
    TypeMap<ComponentBase, SafePtr<ManagerBase>> m_managers;
    TypeMap<ComponentBase, SafePtr<ComponentBase>> m_components;
    std::shared_ptr<EventController> m_eventController;

    TypeIdList m_dependenceComponentList;
    SafePtr<ISubscription> m_updateSubscription;

    void subscribeEvents();

    /// Called when you add a component to an entity
    /// @param entity New entity pointer. Can be nullptr.
    void setParentEntity(Entity *entity, SafePtr<Entity> entitySafePtr);

    bool checkDependencies();

    void tryInit();

    void tryDeinit();

    void addedToEntityInternal();

    void removedFromEntityInternal();

    virtual void addedToEntity() {}

    virtual void removedFromEntity() {}

    /// By default just calls init(). In Manager implementation
    /// it also emits event about themself to dependants.
    virtual void initInternal() = 0;

    /// By default just call deinit(). In Manager implementation
    /// it also emits event about deinitialization to dependants.
    virtual void deinitInternal() = 0;
};

template <typename ComponentT>
bool ComponentBase::isDependancy() const {
    auto managerIter = std::find(m_dependenceComponentList.begin(), m_dependenceComponentList.end(), ComponentT::id());
    return (managerIter != m_dependenceComponentList.end());
}

/// Returns manager if avaliable
/// You can access only managers at the same or higer level of hierarchy.
template <typename ManagerT>
SafePtr<ManagerT> ComponentBase::manager() const
{
    if (!isDependancy<ManagerT>())
        LOGE("%s is not listed in dependencies of %s but requested",
             typeName<ManagerT>().c_str(),
             componentId().name().c_str());
    return m_managers.get<ManagerT>();
}

template <typename FuncT>
SafePtr<ISubscription> ComponentBase::subscribe(FuncT&& func) {
    return events()->subscribe(std::move(func));
}

} // flappy
