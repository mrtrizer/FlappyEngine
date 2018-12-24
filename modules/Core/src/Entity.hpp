#pragma once

#include "ObjectPool.hpp"

#include <vector>
#include <unordered_set>

namespace flappy {

class Hierarchy;

class Entity : public EnableSelfHandle<Entity> {
    friend class Chank; // for construction with deepness
public:
    constexpr static unsigned MaxDepth = 9999;
    
    explicit Entity(Handle<Entity> parent, Handle<Hierarchy> hierarchy) noexcept
        : Entity(parent, hierarchy, 0)
    {}

    template <typename T>
    class HasComplete {
    private:
        template<typename C>
        static std::true_type Test(decltype(std::declval<C>().template setEntity(Handle<Entity>()))*);
        
        template<typename> static std::false_type& Test(...);
        
    public:
        static bool const value = std::is_same_v<decltype(Test<T>(0)), std::true_type>;
    };
    
    template <typename ComponentT>
    Handle<ComponentT> createComponent() {
        // Template lambda calls a member of Hierarchy whithout it's defenition to avoid cross-reference
        auto component = [](auto& hierarchy) {
            if constexpr (constructedWithHierarchy<ComponentT>())
                return hierarchy->template create<ComponentT>(hierarchy);
            else
                return hierarchy->template create<ComponentT>();
        } (m_hierarchy);
        
        auto& componentInList = m_components.emplace_back(std::move(component));
        // Operate weak handle to prevent memory reallocation effects
        const Handle<ComponentT>& componentHandle = static_cast<const StrongHandle<ComponentT>&>(componentInList);
        
        if constexpr (HasComplete<ComponentT>::value)
            componentHandle->setEntity(selfHandle());
        
        return componentHandle;
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
        auto strongHandlePtr = findComponentInternal<ComponentT>(0);
        return strongHandlePtr != nullptr ? *strongHandlePtr : createComponent<ComponentT>();
    }

    template <typename ComponentT>
    Handle<ComponentT> findComponent(unsigned depth = 0) const {
        auto strongHandlePtr = findComponentInternal<ComponentT>(depth);
        return strongHandlePtr ? *strongHandlePtr : Handle<ComponentT>();
    }
    
    template <typename ComponentT>
    std::list<Handle<ComponentT>> findComponents(unsigned depth = 0) const {
        std::list<Handle<ComponentT>> found;
        
        std::copy_if(m_components.begin(), m_components.end(), std::back_inserter(found), [](const auto& strongHandle) {
            return strongHandle.typeId() == getTypeId<ComponentT>();
        });
        if (depth > 0)
            for (const auto& entity : m_entities) {
                auto nextFound = entity->findComponents<ComponentT>(depth - 1);
                found.insert(found.end(), nextFound.begin(), nextFound.end());
            }
        return found;
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
    
    Handle<Entity> parent() { return m_parent; }

private:
    // Order of members is important as it affects order of destruction
    Handle<Entity> m_parent;
    Handle<Hierarchy> m_hierarchy;
    std::vector<AnyStrongHandle> m_components;
    std::vector<StrongHandle<Entity>> m_entities;
    int m_depth = 0;

    template <typename T>
    static constexpr bool constructedWithHierarchy(decltype(T(std::declval<Handle<Hierarchy>>()))* = 0) { return true; }
    
    template <typename T>
    static constexpr bool constructedWithHierarchy(decltype(T())* = 0) { return false; }

    explicit Entity(Handle<Entity> parent, Handle<Hierarchy> hierarchy, int deepness) noexcept
        : m_parent(parent)
        , m_hierarchy(std::move(hierarchy))
        , m_depth(deepness)
    {}

    template <typename ComponentT>
    const StrongHandle<ComponentT>* findComponentInternal(unsigned depth) const noexcept {
        auto componentIter = std::find_if(m_components.begin(), m_components.end(), [](const auto& strongHandle) {
            return strongHandle.typeId() == getTypeId<ComponentT>();
        });
        if (componentIter != m_components.end())
            return static_cast<const StrongHandle<ComponentT>*>(&*componentIter);
        if (depth > 0)
            for (const auto& entity : m_entities) {
                auto result = entity->findComponentInternal<ComponentT>(depth - 1);
                if (result != nullptr)
                    return result;
            }
        return nullptr;
    }
};

} // flappy
