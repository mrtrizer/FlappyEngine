#pragma once

#include <memory>

#include <core/tools.h>
#include <core/managerlist.h>

namespace flappy {

/// Gives index for every Manager
class ManagerCounter {
public:
    ManagerCounter():
        m_curId(m_count) {
        m_count++;
    }
    unsigned id() {return m_curId;}
    static unsigned count() {return m_count;}
private:
    static unsigned m_count;
    const unsigned m_curId = 0;
};

class AbstractManager
{
    friend class ManagerList;
public:
    AbstractManager() = default;
    virtual ~AbstractManager() = default;
    AbstractManager(const AbstractManager&) = delete;
    void operator=(AbstractManager const&) = delete;

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

template <typename Derived>
class Manager: public AbstractManager
{
public:
    static ManagerCounter counter;
};

template <typename Derived>
ManagerCounter Manager<Derived>::counter;

} // flappy
