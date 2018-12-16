#pragma once

#include "ObjectPool.hpp"

#include <vector>

namespace flappy {

class Hierarchy;

class Entity : public EnableSelfHandle<Entity> {
    friend class Chank; // for construction with deepness
public:
    explicit Entity(const Handle<Hierarchy>& hierarchy) noexcept
        : Entity(hierarchy, 0)
    {}

    template <typename ComponentT>
    Handle<ComponentT> createComponent() {
        auto component = [this](auto& hierarchy) {
            if constexpr (constructedWithEntity<ComponentT>())
                return hierarchy->template create<ComponentT>(selfHandle());
            else
                return hierarchy->template create<ComponentT>();
        } (m_hierarchy);

        return static_cast<const StrongHandle<ComponentT>&>(m_components.emplace_back(std::move(component)));
    }

    bool removeComponent(const AnyHandle& handle) {
        auto componentIter = std::find_if(m_components.begin(), m_components.end(), [&handle](const auto& strongHandle) {
            return handle == strongHandle;
        });
        if (componentIter != m_components.end()) {
            m_components.erase(componentIter);
            return true;
        } else {
            return false;
        }
    }

    template <typename ComponentT>
    Handle<ComponentT> component() {
        auto strongHandlePtr = findComponentInternal<ComponentT>();
        return strongHandlePtr != nullptr ? *strongHandlePtr : createComponent<ComponentT>();
    }

    template <typename ComponentT>
    Handle<ComponentT> findComponent() const {
        auto strongHandlePtr = findComponentInternal<ComponentT>();
        return strongHandlePtr ? *strongHandlePtr : Handle<ComponentT>();
    }

    const std::vector<AnyStrongHandle>& components() const noexcept {
        return m_components;
    }

    const std::vector<StrongHandle<Entity>>& entities() const noexcept {
        return m_entities;
    }

    Handle<Entity> createEntity() noexcept;

    Handle<Hierarchy> hierarchy() {
        return m_hierarchy;
    }

    int depth() const noexcept { return m_depth; }

private:
    // Order of members is important as it affects order of destruction
    Handle<Hierarchy> m_hierarchy;
    std::vector<AnyStrongHandle> m_components;
    std::vector<StrongHandle<Entity>> m_entities;
    int m_depth = 0;

    explicit Entity(const Handle<Hierarchy>& hierarchy, int deepness) noexcept
        : m_hierarchy(std::move(hierarchy))
        , m_depth(deepness)
    {}

    template <typename T>
    static constexpr bool constructedWithEntity(decltype(T(std::declval<Handle<Entity>>()))* = 0) { return true; }

    template <typename T>
    static constexpr bool constructedWithEntity(decltype(T())* = 0) { return false; }

    template <typename ComponentT>
    const StrongHandle<ComponentT>* findComponentInternal() const noexcept {
        auto componentIter = std::find_if(m_components.begin(), m_components.end(), [](const auto& strongHandle) {
            return strongHandle.typeId() == getTypeId<ComponentT>();
        });
        return componentIter != m_components.end() ? static_cast<const StrongHandle<ComponentT>*>(&*componentIter) : nullptr;
    }
};

} // flappy
