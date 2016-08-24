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

class AbstractManager
{
    friend class FlappyApp;
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
    const std::weak_ptr<FlappyApp>& flappyApp() const {return m_flappyApp;}
    virtual void update(TimeDelta){}

private:
    void setFlappyApp(std::weak_ptr<FlappyApp> flappyApp) {
        m_flappyApp = flappyApp;
        m_flappyAppPtr = flappyApp.lock().get();
    }

    std::weak_ptr<FlappyApp> m_flappyApp;
    FlappyApp* m_flappyAppPtr; // optimization of MGR
    bool m_initialized = false;

public:
    template <typename Mgr>
    constexpr auto MGR() const -> decltype(m_flappyAppPtr->MGR<Mgr>()) {
        return m_flappyAppPtr->MGR<Mgr>();
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
