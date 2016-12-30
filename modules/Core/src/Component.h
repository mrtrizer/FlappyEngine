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
    Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;
    virtual void update(TimeDelta) {} //TODO: Hide to private
    virtual void init() {} //TODO: Hide to private

    std::shared_ptr<Entity> entity() const { return m_entity.lock(); }
    std::shared_ptr<ManagerList> managerList() { return m_managerList.lock(); }
    std::shared_ptr<EventController> events() {return m_eventController;}

private:
    std::weak_ptr<ManagerList> m_managerList;
    std::weak_ptr<Entity> m_entity;
    std::shared_ptr<EventController> m_eventController;

    void setEntity(std::weak_ptr<Entity> entity) {m_entity = entity;}
    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        if (auto managerListPtr = managerList.lock()) {
            m_eventController = std::make_shared<EventController>(managerListPtr->events()->eventBus());
        }
        m_managerList = managerList;
    }

public:
    template <typename Manager>
    constexpr auto MGR() const -> decltype(m_managerList.lock()->MGR<Manager>()) {
        return m_managerList.lock()->MGR<Manager>();
    }
};

} // flappy
