#ifndef ENTITY_H
#define ENTITY_H

#include <core/component.h>

class Transform;

class Entity: public std::enable_shared_from_this<Entity> {
    friend class Transform;
public:
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> add(Args ... args) {
        auto component = std::make_shared<ComponentT>(args...);
        component->m_entity = shared_from_this();
        component->init();
        m_components.push_back(component);
        return component;
    }

    //TODO: How to optomize? Dynamic cast for every component is bad idea.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> get() const {
        for (auto component: m_components) {
            auto cast = std::dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                return cast;
        }
        return nullptr;
    }

    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> getAll() const {
        std::size_t hash = typeid(ComponentT).hash_code();
        std::list<std::shared_ptr<ComponentT>> list;
        for (auto component: m_components) {
            auto cast = std::dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                list.push_back(std::dynamic_pointer_cast<ComponentT>(cast));
        }
        return list;
    }

    void update(TimeDelta dt) {
        for (auto component: m_components)
            component->update(dt);
    }

    std::shared_ptr<Transform> transform() { return m_transform; }

private:
    std::list<std::shared_ptr<Component>> m_components;
    std::shared_ptr<Transform> m_transform;
};

#endif // ENTITY_H
