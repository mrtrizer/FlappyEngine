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

private:

    void setEntity(std::weak_ptr<Entity> entity) {m_entity = entity;}
    std::weak_ptr<Entity> m_entity;

    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        m_managerList = managerList;
    }

    std::weak_ptr<ManagerList> m_managerList;

public:
    template <typename Manager>
    constexpr auto MGR() const -> decltype(m_managerList.lock()->MGR<Manager>()) {
        return m_managerList.lock()->MGR<Manager>();
    }
};

} // flappy
