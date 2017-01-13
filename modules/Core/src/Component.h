#pragma once

#include <memory>

#include <ManagerList.h>
#include <Tools.h>

namespace flappy {

class Entity;
class TransformComponent;

class Component {
    friend class ManagerList;
    friend class Entity;
public:
    Component():
        m_eventController(std::make_shared<EventController>())
    {}
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;
#warning TODO: Hide to private
    virtual void update(TimeDelta) {}
#warning TODO: Hide to private
    virtual void init() {}

    std::shared_ptr<Entity> entity() const { return m_entity.lock(); }
    std::shared_ptr<ManagerList> managerList() { return m_managerList.lock(); }
    std::shared_ptr<EventController> events() { return m_eventController; }
    template <typename ComponentT>
    std::shared_ptr<ComponentT> component() {
        return [](auto entity) {
            return entity->component<ComponentT>();
        } (entity());
    }

private:
    std::weak_ptr<ManagerList> m_managerList;
    std::weak_ptr<Entity> m_entity;
    std::shared_ptr<EventController> m_eventController;

    void setEntity(std::weak_ptr<Entity> entity) {
        m_entity = entity;
    }
    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        m_managerList = managerList;
    }

public:
    template <typename ManagerT>
    auto manager() const -> decltype(m_managerList.lock()->manager<ManagerT>()) {
        return m_managerList.lock()->manager<ManagerT>();
    }
};

} // flappy
