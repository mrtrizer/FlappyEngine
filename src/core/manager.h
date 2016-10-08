#pragma once

#include <memory>

#include <core/tools.h>
#include <core/managerlist.h>
#include <core/classid.h>

namespace flappy {

class Manager
{
    friend class ManagerList;
public:
    Manager() = default;
    virtual ~Manager() = default;
    Manager(const Manager&) = delete;
    void operator=(Manager const&) = delete;

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
    constexpr auto MGR() const -> decltype(m_managerList.lock()->MGR<Mgr>()) {
        auto managerListSPtr = m_managerList.lock();
        if (!managerListSPtr)
            throw std::runtime_error("This Manager should be created inside ManagerList. Use ManagerList::create<ManagerT>() function.");
        return managerListSPtr->MGR<Mgr>();
    }
};

} // flappy
