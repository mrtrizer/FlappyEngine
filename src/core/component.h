#pragma once

#include <memory>

#include <core/managerlist.h>
#include <core/tools.h>

namespace flappy {

class Entity;

class Component {
    friend class ManagerList;
    friend class Entity;
public:
    Component(){}
    Component(const Component&) = delete;
    void operator=(const Component&) = delete;
    virtual ~Component() {}
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
    ManagerList* m_managerListPtr; // optimization of MGR

public:
    template <typename Mgr>
    constexpr auto MGR() const -> decltype(m_managerListPtr->MGR<Mgr>()) {
        return m_managerListPtr->MGR<Mgr>();
    }
};

} // flappy
