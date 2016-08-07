#pragma once

#include <memory>

#include <core/flappyapp.h>
#include <core/tools.h>

namespace flappy {

using namespace std;

class Entity;

class Component {
    friend class FlappyApp;
    friend class Entity;
public:
    Component(){}
    Component(const Component&) = delete;
    void operator=(const Component&) = delete;
    virtual ~Component() {}
    virtual void update(TimeDelta) {}
    virtual void init() {}

    shared_ptr<Entity> entity() const { return m_entity.lock(); }

protected:
    weak_ptr<FlappyApp> flappyApp() const {return m_flappyApp;}

private:
    void setFlappyApp(weak_ptr<FlappyApp> flappyApp) {
        m_flappyApp = flappyApp;
        m_flappyAppPtr = flappyApp.lock().get();
    }
    void setEntity(weak_ptr<Entity> entity) {m_entity = entity;}

    weak_ptr<FlappyApp> m_flappyApp;
    FlappyApp* m_flappyAppPtr; // optimization of MGR
    weak_ptr<Entity> m_entity;
public:
    template <typename Mgr> inline
    constexpr auto MGR() const -> decltype(m_flappyAppPtr->MGR<Mgr>()) {
        return m_flappyAppPtr->MGR<Mgr>();
    }
};

} // flappy