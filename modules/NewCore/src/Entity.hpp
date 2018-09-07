#pragma once

#include "ObjectPool.hpp"

#include <vector>

namespace flappy {

class Hierarchy;

class Entity : public EnableSelfHandle<Entity> {
public:
    explicit Entity(const Handle<Hierarchy>& hierarchy) noexcept
        : m_hierarchy(std::move(hierarchy))
    {}

    template <typename ComponentT>
    const StrongHandle<ComponentT>& createComponent() {
        auto component = [this](auto& hierarchy) {
            if constexpr (constructedWithEntity<ComponentT>())
                return hierarchy->template create<ComponentT>(selfHandle());
            else
                return hierarchy->template create<ComponentT>();
        } (m_hierarchy);
        if constexpr (hasUpdate<ComponentT>(0)) {
            m_updateFunctions.emplace_back([componentHandle = component.handle()](float dt) {
               componentHandle->update(dt);
            });
        }

        return static_cast<const StrongHandle<ComponentT>&>(m_components.emplace_back(std::move(component)));
    }

    template <typename ComponentT>
    const StrongHandle<ComponentT>& component() {
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

    const std::vector<std::function<void(float)>>& updateFunctions() const noexcept {
        return m_updateFunctions;
    }


private:
    Handle<Hierarchy> m_hierarchy;
    std::vector<AnyStrongHandle> m_components;
    std::vector<StrongHandle<Entity>> m_entities;
    std::vector<std::function<void(float)>> m_updateFunctions;

    template <typename ComponentT>
    static constexpr bool hasUpdate(...) { return false; }

    template <typename ComponentT>
    static constexpr bool hasUpdate(int, decltype((std::declval<ComponentT>().update(float() )))* = 0) { return true; }

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
