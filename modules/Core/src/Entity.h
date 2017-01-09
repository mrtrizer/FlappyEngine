#pragma once

#include <list>

#include <Tools.h>

#include "EventController.h"
#include "Component.h"

namespace flappy {

class TransformComponent;
class Builder;

class Entity: public std::enable_shared_from_this<Entity> {
    friend class TransformComponent;
public:
    Entity(std::weak_ptr<ManagerList> managerList):
        m_managerList(managerList),
        m_eventController(std::make_shared<EventController>())
    {}
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> create(Args ... args) {
        using namespace std;
        auto component = make_shared<ComponentT>(args...);
        component->setEntity(shared_from_this());
        component->setManagerList(managerList());
        component->init();
        m_components.push_back(component);
        return component;
    }

#warning TODO: Rename to component()
    // TODO: How to optomize? Dynamic cast for every component is bad idea.
    // We can store all presenters in separate list
    template<typename ComponentT>
    std::shared_ptr<ComponentT> get() const {
        using namespace std;
        for (auto component: m_components) {
            auto cast = dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                return cast;
        }
        return nullptr;
    }

    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> getAll() const {
        using namespace std;
        std::list<std::shared_ptr<ComponentT>> list;
        for (auto component: m_components) {
            auto cast = dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                list.push_back(dynamic_pointer_cast<ComponentT>(cast));
        }
        return list;
    }

    void update(TimeDelta dt);

    std::shared_ptr<Entity> add(const Builder &builder);

    std::shared_ptr<EventController> events() {return m_eventController;}

    std::shared_ptr<TransformComponent> transform() { return m_transform; }

    std::weak_ptr<ManagerList> managerList() const {return m_managerList;}

private:
    std::list<std::shared_ptr<Component>> m_components;
    std::shared_ptr<TransformComponent> m_transform;
    std::shared_ptr<EventController> m_eventController;

    std::weak_ptr<ManagerList> m_managerList;
};

} // flappy
