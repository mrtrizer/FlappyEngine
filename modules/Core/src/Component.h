#pragma once

#include <memory>

// To not include everywhere
#include <EventController.h>

namespace flappy {

class Entity;
class EventController;

class Component {
    friend class Entity;
public:
    Component();
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;

    std::weak_ptr<Entity> entity() const { return m_entity; }
    std::shared_ptr<EventController> events() { return m_eventController; }

    virtual void update(float) {}
    virtual void init() {}
    virtual void deinit() {}

protected:
    template <typename ComponentT>
    std::shared_ptr<ComponentT> component() {
        return [](auto entity) {
            return entity.lock()->component<ComponentT>();
        } (entity());
    }

    template <typename ManagerT>
    std::shared_ptr<ManagerT> manager() const {
        return [](auto entity) {
            return entity.lock()->manager<ManagerT>();
        } (entity());
    }

private:
    std::weak_ptr<Entity> m_entity;
    std::shared_ptr<EventController> m_eventController;

    void setEntity(std::weak_ptr<Entity> entity) {
        m_entity = entity;
    }
};

} // flappy
