#pragma once

#include <memory>

// To not include everywhere
#include <EventController.h>
#include <SafePtr.h>
#include <Tools.h>

namespace flappy {

class Manager;
class Entity;
class EventController;

class Component: public std::enable_shared_from_this<Component> {
    friend class Entity;
public:
    Component();
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;

    virtual void update(DeltaTime) {}

    /// Returns parent entity (can be null if conponent is not added to entity)
    SafePtr<Entity> entity() const { return m_entity; }

    /// Returns EventController
    std::shared_ptr<EventController> events() { return m_eventController; }

protected:
    template <typename ManagerT>
    SafePtr<ManagerT> manager() const {
        return m_managers.get<ManagerT>();
    }

    template <typename T>
    std::shared_ptr<T> selfSharedPointer() {
        return std::static_pointer_cast<T>(shared_from_this());
    }

    template <typename T>
    SafePtr<T> selfPointer() {
        return selfSharedPointer<T>();
    }

protected:
    /// Called when you already added to entity.
    /// @details You have access to parent entity from this method first time.
    virtual void init() {}

    /// Called when you removed from the entity.
    /// @details Here, you have last chance to access to your past neighbors and entity
    virtual void deinit() {}

private:
    SafePtr<Entity> m_entity;
    TypeMap<Component, SafePtr<Manager>> m_managers;
    std::shared_ptr<EventController> m_eventController;

    /// Called when you add a component to an entity
    /// @param entity New entity pointer. Can be nullptr.
    void setParentEntity(SafePtr<Entity> entity);

    virtual void initInternal() {}

    virtual void deinitInternal() {}
};

} // flappy
