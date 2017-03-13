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

class Component {
    friend class Entity;
public:
    struct OnManagerAdded: IEvent {
        unsigned id;
        SafePtr<Manager> pointer;
    };

    struct OnManagerRemoved: IEvent {
        unsigned id;
        SafePtr<Manager> pointer;
    };

    Component();
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;

    virtual void init() {}

    virtual void update(flappy::TimeDelta) {}

    SafePtr<Entity> entity() const { return m_entity; }
    std::shared_ptr<EventController> events() { return m_eventController; }

protected:
    template <typename ComponentT>
    SafePtr<ComponentT> component() {
        return [](auto entity) {
            return entity->component<ComponentT>();
        } (entity());
    }

    template <typename ManagerT>
    SafePtr<ManagerT> manager() const {
        return m_managers.get<ManagerT>();
    }

private:
    SafePtr<Entity> m_entity;
    TypeMap<Component, SafePtr<Manager>> m_managers;
    std::shared_ptr<EventController> m_eventController;

    void setEntity(SafePtr<Entity> entity) {
        m_entity = entity;
    }
};

} // flappy
