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
    Entity():
        m_eventBus(std::make_shared<EventBus>())
    {}
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    void setManagerList(std::weak_ptr<ManagerList> managerList) {
        if (!m_managerList.expired())
            throw std::runtime_error("You can't add entity to several entity managers.");
        m_managerList = managerList;
        if (auto managerListPtr = m_managerList.lock())
            managerListPtr->events()->eventBus()->addChild(m_eventBus);
        for (auto component: m_components) {
            component->setManagerList(m_managerList);
            component->init();
        }
    }

    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> create(Args ... args) {
        using namespace std;
        auto component = make_shared<ComponentT>(args...);
        component->setEntity(shared_from_this());
        component->events()->setEventBus(eventBus());
        if (!managerList().expired()) {
            component->setManagerList(managerList());
            component->init();
        }
        m_components.push_back(component);
        return component;
    }

    template<typename ComponentT>
    std::shared_ptr<ComponentT> findComponent() {
        using namespace std;
        for (auto component: m_components) {
            auto cast = dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                return cast;
        }
        return nullptr;
    }

#warning TODO: Rename to component()
    // TODO: How to optomize? Dynamic cast for every component is bad idea.
    // We can store all presenters in separate list
    template<typename ComponentT>
    std::shared_ptr<ComponentT> component() {
        if (auto foundComponent = findComponent<ComponentT>())
            return foundComponent;
        return create<ComponentT>();
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

    std::shared_ptr<EventBus> eventBus() {return m_eventBus;}

    std::shared_ptr<TransformComponent> transform();

    std::weak_ptr<ManagerList> managerList() const {return m_managerList;}

private:
    std::list<std::shared_ptr<Component>> m_components;
    std::shared_ptr<TransformComponent> m_transform;
    std::shared_ptr<EventBus> m_eventBus;

    std::weak_ptr<ManagerList> m_managerList;
};

} // flappy
