#pragma once

#include <memory>

#include "Tools.h"
#include "ManagerList.h"

namespace flappy {

class BaseManager
{
    friend class ManagerList;
public:
    BaseManager() = default;
    virtual ~BaseManager() = default;
    BaseManager(const BaseManager&) = delete;
    void operator=(BaseManager const&) = delete;

protected:
    const std::weak_ptr<ManagerList>& managerList() const {return m_managerList;}
    virtual void update(TimeDelta){}
    virtual void init(){}

private:
    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        m_managerList = managerList;
    }

    std::weak_ptr<ManagerList> m_managerList;

public:
    template <typename Mgr>
    auto MGR() const -> decltype(m_managerList.lock()->MGR<Mgr>()) {
        auto managerListSPtr = m_managerList.lock();
        if (!managerListSPtr)
            throw std::runtime_error("This Manager should be created inside ManagerList. Use ManagerList::create<ManagerT>() function.");
        return managerListSPtr->MGR<Mgr>();
    }
};

using Manager = BaseManager;

} // flappy
