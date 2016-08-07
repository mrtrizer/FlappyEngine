#pragma once

#include "component.h"

namespace flappy {

using namespace std;

class Transform;

class Entity: public enable_shared_from_this<Entity> {
    friend class Transform;
public:
    Entity(weak_ptr<FlappyApp> flappyApp):m_flappyApp(flappyApp){}
    Entity(const Entity&) = delete;
    void operator=(const Entity&) = delete;

    template <typename ComponentT, typename ... Args>
    shared_ptr<ComponentT> add(Args ... args) {
        auto component = make_shared<ComponentT>(args...);
        component->setEntity(shared_from_this());
        component->setFlappyApp(flappyApp());
        component->init();
        m_components.push_back(component);
        return component;
    }

    // TODO: How to optomize? Dynamic cast for every component is bad idea.
    // We can store all presenters in separate list
    template<typename ComponentT>
    shared_ptr<ComponentT> get() const {
        for (auto component: m_components) {
            auto cast = dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                return cast;
        }
        return nullptr;
    }

    template<typename ComponentT>
    list<shared_ptr<ComponentT>> getAll() const {
        size_t hash = typeid(ComponentT).hash_code();
        list<shared_ptr<ComponentT>> list;
        for (auto component: m_components) {
            auto cast = dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                list.push_back(dynamic_pointer_cast<ComponentT>(cast));
        }
        return list;
    }

    // TODO: Move to cpp
    void update(TimeDelta dt) {
        for (auto component: m_components)
            component->update(dt);
    }

    shared_ptr<Transform> transform() { return m_transform; }

protected:
    weak_ptr<FlappyApp> flappyApp() const {return m_flappyApp;}

private:
    list<shared_ptr<Component>> m_components;
    shared_ptr<Transform> m_transform;

    weak_ptr<FlappyApp> m_flappyApp;
};

} // flappy
