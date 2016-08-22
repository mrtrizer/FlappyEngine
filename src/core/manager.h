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

//TODO: Rename. It's not an interface anymore
class IManager
{
    friend class FlappyApp;
public:
    IManager() {}

    IManager(const IManager&) = delete;
    void operator=(IManager const&) = delete;

    virtual void update(TimeDelta){}
    virtual void init(){}

protected:
    const std::weak_ptr<FlappyApp>& flappyApp() const {return m_flappyApp;}

private:
    void setFlappyApp(std::weak_ptr<FlappyApp> flappyApp) {
        m_flappyApp = flappyApp;
        m_flappyAppPtr = flappyApp.lock().get();
    }

    std::weak_ptr<FlappyApp> m_flappyApp;
    FlappyApp* m_flappyAppPtr; // optimization of MGR

public:
    template <typename Mgr>
    constexpr auto MGR() const -> decltype(m_flappyAppPtr->MGR<Mgr>()) {
        return m_flappyAppPtr->MGR<Mgr>();
    }
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
