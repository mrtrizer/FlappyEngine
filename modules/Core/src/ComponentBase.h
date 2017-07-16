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

class ComponentBase: public std::enable_shared_from_this<ComponentBase>
{
    friend class Entity;
public:
    struct OnUpdate: public IEvent
    {
        DeltaTime dt;
    };

    using ClassIdList = std::list<unsigned>;

    ComponentBase(ClassIdList dependenceClassIdList);
    ComponentBase();
    ComponentBase(const ComponentBase&) = delete;
    ComponentBase& operator=(const ComponentBase&) = delete;
    ComponentBase(ComponentBase&&) = default;
    ComponentBase& operator=(ComponentBase&&) & = default;
    virtual ~ComponentBase();

    virtual unsigned componentId() = 0;

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
        unsigned id;
        SafePtr<ManagerBase> pointer;
        template <typename ManagerT>
        SafePtr<ManagerT> castTo() {
            if (ClassId<ComponentBase, ManagerT>::id() == id)
                return SafePtr<ManagerT>(pointer);
            else
                return SafePtr<ManagerT>();
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
    bool isManagerRegistered(unsigned id);

    bool isComponentRegistered(unsigned id);

    /// Returns manager if avaliable
    template <typename ManagerT>
    SafePtr<ManagerT> manager() const
    {
        return m_managers.get<ManagerT>();
    }

    /// Returns shared_ptr<Component> static casted to shared_ptr<T>
    template <typename T>
    std::shared_ptr<T> selfSharedPointer()
    {
        return std::static_pointer_cast<T>(shared_from_this());
    }

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

    bool allDependenciesReady();

    /// Called when you already added to entity.
    /// @details You have access to parent entity from this method first time.
    virtual void init()
    {}

    /// Called when you removed from the entity.
    /// @details Here, you have last chance to access to your past neighbors and entity
    virtual void deinit()
    {}

private:
    bool m_initializedFlag = false;
    SafePtr<Entity> m_entity;
    TypeMap<ComponentBase, SafePtr<ManagerBase>> m_managers;
    std::shared_ptr<EventController> m_eventController;

    ClassIdList m_dependenceClassIdList;
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

    virtual void addedToEntity()
    {}

    virtual void removedFromEntity()
    {}

    /// By default just calls init(). In Manager implementation
    /// it also emits event about themself to dependants.
    virtual void initInternal();

    /// By default just call deinit(). In Manager implementation
    /// it also emits event about deinitialization to dependants.
    virtual void deinitInternal();
};

} // flappy
