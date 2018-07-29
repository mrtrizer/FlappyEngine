#pragma once

#include "ObjectPool.hpp"

#include <vector>

class Hierarchy;

class Entity : public EnableSelfHandle<Entity> {
public:
    explicit Entity(Handle<Hierarchy> systems)
    {
    }

    template <typename ComponentT>
    Handle<ComponentT> component() {
        auto componentIter = m_components.find_if([](const auto& strongHandle) {
            return strongHandle.id() == getTypeId<ComponentT>();
        });
        if (componentIter == m_components.end()) {
            auto component = m_objectPool->create<ComponentT>(selfHandle());
            m_components.emplace_back(component);
        }
        return *componentIter;
    }

private:
    Handle<Hierarchy> m_hierarchy;
    std::vector<UnknownStrongHandle> m_components;
};
