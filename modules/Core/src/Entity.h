#pragma once

#include <list>

#include "EventController.h"
#include "Component.h"

namespace flappy {

class ManagerList;

class Entity: public std::enable_shared_from_this<Entity> {
    friend class TransformComponent;
public:
    Entity():
        m_eventBus(std::make_shared<EventBus>())
    {}
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    // Remove and create setParent instead. Take manager list from parent.
    void setManagerList(std::weak_ptr<ManagerList> managerList);

    void setParent(std::weak_ptr<Entity> parent) {
        m_parent = parent;
    }

    std::weak_ptr<Entity> parent() {
        return m_parent;
    }

    /// @brief Returns first component of required type or null if
    /// it doesn't exist.
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

    /// @brief Returns first component of required type else creates new component.
    /// @details Useful for default component managment. If only one component
    /// of same type avaliable at same time.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> component() {
        if (auto foundComponent = findComponent<ComponentT>())
            return foundComponent;
        return createComponent<ComponentT>();
    }

    /// @brief Returns all components of required type.
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> components() const {
        using namespace std;
        std::list<std::shared_ptr<ComponentT>> list;
        for (auto component: m_components) {
            auto cast = dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                list.push_back(dynamic_pointer_cast<ComponentT>(cast));
        }
        return list;
    }

    /// @brief Useful for manual control of component creation.
    /// @details Use to create components with constructor with arguments.
    /// Also can be used to create multiple components of same type in
    /// single entity.
    /// @see findComponent(), components()
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> createComponent(Args ... args) {
        using namespace std;
        auto component = make_shared<ComponentT>(args...);
        component->setEntity(shared_from_this());
        component->events()->setEventBus(eventBus());
        if (!managerList().expired()) {
            component->init();
        }
        m_components.push_back(component);
        return component;
    }

    /// @brief Pass update to all components and child entities
    void update(float dt);

    /// @brief Returns pointer to entity EventBus.
    std::shared_ptr<EventBus> eventBus() {return m_eventBus;}

    /// @brief Returns current managerList.
    std::weak_ptr<ManagerList> managerList() const {return m_managerList;}

    std::shared_ptr<Entity> createEntity();
    std::shared_ptr<Entity> addEntity(std::shared_ptr<Entity> entity);

private:
    std::list<std::shared_ptr<Component>> m_components;
    std::shared_ptr<EventBus> m_eventBus;
    std::weak_ptr<ManagerList> m_managerList;
    std::weak_ptr<Entity> m_parent;
};

} // flappy
