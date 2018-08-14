#pragma once

#include "ObjectPool.hpp"
#include "Hierarchy.hpp"

#include <vector>

class Hierarchy;

class Entity : public EnableSelfHandle<Entity> {
public:
    explicit Entity(Handle<Hierarchy> hierarchy) noexcept
        : m_hierarchy(std::move(hierarchy))
    {}

    template <typename ComponentT>
    const StrongHandle<ComponentT>& createComponent() {
        auto component = m_hierarchy->create<ComponentT>(selfHandle());
        if constexpr (hasUpdate<ComponentT>())
            component->update();

        return static_cast<const StrongHandle<ComponentT>&>(m_components.emplace_back());
    }

    template <typename ComponentT>
    const StrongHandle<ComponentT>& component() {
        auto strongHandlePtr = findComponentInternal<ComponentT>();
        return strongHandlePtr != nullptr ? &strongHandlePtr : createComponent<ComponentT>();
    }

    template <typename ComponentT>
    Handle<ComponentT> findComponent() const {
        auto strongHandlePtr = findComponentInternal<ComponentT>();
        return strongHandlePtr != nullptr ? *strongHandlePtr : nullptr;
    }

    const std::vector<AnyStrongHandle>& components() const noexcept {
        return m_components;
    }

    const std::vector<StrongHandle<Entity>>& children() const noexcept {
        return m_children;
    }

private:
    Handle<Hierarchy> m_hierarchy;
    std::vector<AnyStrongHandle> m_components;
    std::vector<StrongHandle<Entity>> m_children;

    template <typename ComponentT>
    constexpr bool hasUpdate() { return false; }

    template <typename ComponentT, void (ComponentT::*update)(float)>
    constexpr bool hasUpdate() { return true; }

    template <typename ComponentT>
    const StrongHandle<ComponentT>* findComponentInternal() const noexcept {
        auto componentIter = std::find_if(m_components.begin(), m_components.end(), [](const auto& strongHandle) {
            return strongHandle.id() == getTypeId<ComponentT>();
        });
        return componentIter != m_components.end();
    }
};
