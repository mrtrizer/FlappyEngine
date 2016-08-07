#pragma once

#include <memory>

#include <core/tools.h>
#include <core/flappyapp.h>

namespace flappy {

/// Gives index for every Manager
class ManagerCounter {
public:
    ManagerCounter():
        m_curId(m_count) {
        m_count++;
    }
    int id() {return m_curId;}
    static int count() {return m_count;}
private:
    static int m_count;
    int m_curId = 0;
};

class IManager
{
public:
    IManager() {}

    IManager(const IManager&) = delete;
    void operator=(IManager const&) = delete;

    template <typename Mgr>
    constexpr auto MGR() const -> decltype(FlappyApp::inst().MGR<Mgr>()) {
        return FlappyApp::inst().MGR<Mgr>();
    }

    virtual void update(TimeDelta){}
    virtual void init(){}
};

template <typename Derived>
class Manager: public IManager
{
public:
    static ManagerCounter counter;
};

template <typename Derived>
ManagerCounter Manager<Derived>::counter;

} // flappy
