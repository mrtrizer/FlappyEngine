#pragma once

#include <memory>

#include <core/managerlist.h>
#include <core/tools.h>

namespace flappy {

class Entity;
class Transform;

class Component {
    friend class ManagerList;
    friend class Entity;
public:
    Component() = default;
    Component(const Component&) = delete;
    Component& operator=(const Component&) = delete;
    virtual ~Component() = default;
    virtual void update(TimeDelta) {}
    virtual void init() {}

    std::shared_ptr<Entity> entity() const { return m_entity.lock(); }

private:

    void setEntity(std::weak_ptr<Entity> entity) {m_entity = entity;}
    std::weak_ptr<Entity> m_entity;

    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        m_managerList = managerList;
        m_managerListPtr = managerList.lock().get();
    }

    std::weak_ptr<ManagerList> m_managerList;
    ManagerList* m_managerListPtr = nullptr; // optimization of MGR

public:
    template <typename Manager>
    constexpr auto MGR() const -> decltype(m_managerListPtr->MGR<Manager>()) {
        return m_managerListPtr->MGR<Manager>();
    }
};

} // flappy
