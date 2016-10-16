#pragma once

#include <memory>

#include "tools.h"
#include "managerlist.h"
#include "classid.h"
#include "dependent.h"

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
    static bool satisfied(std::function<bool(unsigned)>) {
        return true;
    }
    virtual bool checkSatisfied(std::function<bool(unsigned)> check) = 0;

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

template <typename ... Dependency>
class Manager: public BaseManager, public Dependent<BaseManager, Dependency...>
{
public:
    static bool satisfied(std::function<bool(unsigned)> check) {
        return Dependent<BaseManager, Dependency...>::dependenceSatisfied(check);
    }
private:
    bool checkSatisfied(std::function<bool(unsigned)> check) override {
        return satisfied(check);
    }
};

} // flappy
