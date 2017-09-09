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
/// Component can be in two states:
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

    ComponentBase();
    ComponentBase(const ComponentBase&) = delete;
    ComponentBase& operator=(const ComponentBase&) = delete;
    ComponentBase(ComponentBase&&) = default;
    ComponentBase& operator=(ComponentBase&&) & = default;
    virtual ~ComponentBase();

    virtual TypeId<ComponentBase> componentId() const = 0;

    /// Returns parent entity (can be null if conponent is not added to entity)
    SafePtr<Entity> entity() const
    {
        return m_entity;
    }

    /// Returns EventController
    std::shared_ptr<EventController> events()
    {
        return m_eventController;
    }

    bool isInitialized() { return m_initializedFlag; }

    struct ComponentEvent: public IEvent
    {
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
    {};

    struct ManagerRemovedEvent: public ComponentEvent
    {};

    struct ComponentAddedEvent: public ComponentEvent
    {};

    struct ComponentRemovedEvent: public ComponentEvent
    {};

protected:
    bool isManagerRegistered(TypeId<ComponentBase> id);

    bool isComponentRegistered(TypeId<ComponentBase> id);

    /// Returns manager if avaliable
    template <typename ManagerT>
    SafePtr<ManagerT> manager() const
    {
        auto managerIter = std::find(m_dependenceComponentList.begin(), m_dependenceComponentList.end(), ManagerT::id());
        if (managerIter == m_dependenceComponentList.end())
            LOGE("%s is not listed in dependencies of %s but requested", typeName<ManagerT>().c_str(), componentId().name().c_str());
        return m_managers.get<ManagerT>();
    }

    /// Returns shared_ptr<Component> static casted to shared_ptr<T>
    template <typename T>
    std::shared_ptr<T> selfSharedPointer()
    {
        return std::static_pointer_cast<T>(shared_from_this());
    }

    /// Returns shared_ptr<Component> static casted to shared_ptr<T>
    template <typename T>
    const std::shared_ptr<T> selfSharedPointer() const
    {
        return std::static_pointer_cast<T>(shared_from_this());
    }

    /// Returns SafePtr<Component> static casted to SafePtr<T>
    template <typename T>
    SafePtr<T> selfPointer()
    {
        return selfSharedPointer<T>();
    }

    /// Returns SafePtr<Component> static casted to SafePtr<T>
    template <typename T>
    const SafePtr<T> selfPointer() const
    {
        return selfSharedPointer<T>();
    }

    // TODO: Deprecated. Remove update methods. Listen OnUpdate event instead.
    virtual void update(DeltaTime)
    {}

    bool allComponentsReady();

    // TODO: Deprecated. Remove update methods. Listen OnInit event instead.
    /// Called when you already added to entity.
    /// @details You have access to parent entity from this method first time.
    virtual void init()
    {}

    // TODO: Deprecated. Remove update methods. Listen Deinit event instead.
    /// Called when you removed from the entity.
    /// @details Here, you have last chance to access to your past neighbors and entity
    virtual void deinit()
    {}

    void addDependency(TypeId<ComponentBase> id);

private:
    using TypeIdList = std::list<TypeId<ComponentBase>>;

    bool m_initializedFlag = false;
    SafePtr<Entity> m_entity;
    TypeMap<ComponentBase, SafePtr<ManagerBase>> m_managers;
    TypeMap<ComponentBase, SafePtr<ComponentBase>> m_components;
    std::shared_ptr<EventController> m_eventController;

    TypeIdList m_dependenceComponentList;
    std::weak_ptr<ISubscription> m_updateSubscription;

    void subscribeEvents();

    /// Called when you add a component to an entity
    /// @param entity New entity pointer. Can be nullptr.
    void setParentEntity(SafePtr<Entity> entity);

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

} // flappy
