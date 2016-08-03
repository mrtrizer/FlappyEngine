#pragma once

#include "component.h"

namespace flappy {

using namespace std;

class Transform;

class Entity: public enable_shared_from_this<Entity> {
    friend class Transform;
public:
    Entity(){}
    Entity(const Entity&) = delete;
    void operator=(const Entity&) = delete;

    template <typename ComponentT, typename ... Args>
    shared_ptr<ComponentT> add(Args ... args) {
        auto component = make_shared<ComponentT>(args...);
        component->m_entity = shared_from_this();
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

private:
    list<shared_ptr<Component>> m_components;
    shared_ptr<Transform> m_transform;
};

} // flappy
