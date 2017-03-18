#pragma once

#include <memory>

// To not include everywhere
#include <EventController.h>
#include <SafePtr.h>
#include <Tools.h>

namespace flappy
{

class Manager;
class Entity;
class EventController;

class Component: public std::enable_shared_from_this<Component>
{
    friend class Entity;
public:
    struct OnUpdate: public IEvent
    {
        DeltaTime dt;
    };

    Component();
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;

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

protected:
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

protected:
    /// Called when you already added to entity.
    /// @details You have access to parent entity from this method first time.
    virtual void init()
    {}

    /// Called when you removed from the entity.
    /// @details Here, you have last chance to access to your past neighbors and entity
    virtual void deinit()
    {}

private:
    SafePtr<Entity> m_entity;
    TypeMap<Component, SafePtr<Manager>> m_managers;
    std::shared_ptr<EventController> m_eventController;

    /// Called when you add a component to an entity
    /// @param entity New entity pointer. Can be nullptr.
    void setParentEntity(SafePtr<Entity> entity);

    virtual void initInternal()
    {}

    virtual void deinitInternal()
    {}
};

} // flappy
