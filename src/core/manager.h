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
    unsigned m_curId = 0;
};

class AbstractManager
{
    friend class ManagerList;
public:
    AbstractManager() {}

    AbstractManager(const AbstractManager&) = delete;
    void operator=(AbstractManager const&) = delete;

    void tryInit()
    {
        if (!m_initialized) {
            init();
            m_initialized = true;
        }
    }
    virtual void init(){}

protected:
    const std::weak_ptr<ManagerList>& managerList() const {return m_managerList;}
    virtual void update(TimeDelta){}

private:
    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        m_managerList = managerList;
        m_managerListPtr = managerList.lock().get();
    }

    std::weak_ptr<ManagerList> m_managerList;
    ManagerList* m_managerListPtr; // optimization of MGR
    bool m_initialized = false;

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
