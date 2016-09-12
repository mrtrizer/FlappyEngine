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
        m_managerListPtr = managerList.lock().get();
    }

    std::weak_ptr<ManagerList> m_managerList;
    ManagerList* m_managerListPtr = nullptr; // optimization of MGR

public:
    template <typename Mgr>
    constexpr auto MGR() const -> decltype(m_managerListPtr->MGR<Mgr>()) {
        return m_managerListPtr->MGR<Mgr>();
    }
};

} // flappy
