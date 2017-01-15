#pragma once

#include <memory>

// To not include everywhere
#include <EventController.h>

namespace flappy {

class Entity;
class EventController;
class ManagerList;

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
            return entity.lock()->managerList().lock()->manager<ManagerT>();
        } (entity());
    }

    /// @brief Returns a weak pointer to the current manager list.
    /// @details I return a weak_ptr to say "Saving of a pointer is a bad idea".
    std::weak_ptr<ManagerList> managerList();

private:
    std::weak_ptr<Entity> m_entity;
    std::shared_ptr<EventController> m_eventController;

    void setEntity(std::weak_ptr<Entity> entity) {
        m_entity = entity;
    }
};

} // flappy
