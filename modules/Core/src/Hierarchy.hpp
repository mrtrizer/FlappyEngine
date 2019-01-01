#pragma once

#include <array>
#include <unordered_set>
#include <AnyStrongHandle.hpp>
#include <MemoryManager.hpp>
#include "ObjectPool.hpp"
#include "Entity.hpp"

namespace flappy {

class Entity;

class Hierarchy : public EnableSelfHandle<Hierarchy> {
    friend class Entity; // for postponed entity removing via addRemovedEntity()
public:
    Hierarchy(MemoryManager& memoryManager)
        : m_rootEntity(memoryManager.create<Entity>(nullptr, selfHandle(), memoryManager))
        , m_memoryManager(memoryManager)
    {}

    ~Hierarchy() = default;

    template <typename ManagerT>
    Handle<ManagerT> manager() {
        auto typeId = getTypeId<ManagerT>();
        try {
            return m_managerHandles.at(typeId);
        } catch (const std::exception&) {
            throw FlappyException("Can't find manager " + typeId.name);
        }
    }

    MemoryManager& memoryManager() { return m_memoryManager; }

    Handle<Entity> rootEntity() {
        return m_rootEntity;
    }

    template <typename ManagerT, typename DerivedT = ManagerT>
    Handle<DerivedT> initManager() {
        static_assert(!std::is_abstract<DerivedT>(), "Can't construct manager of abstract type.");
        
        auto managerIter = m_managerHandles.find(getTypeId<ManagerT>());
        auto derivedIter = m_managerHandles.find(getTypeId<DerivedT>());
        auto iter = managerIter;
        if (iter == m_managerHandles.end())
            iter = derivedIter;
        
        auto strongIter = m_managers.end();
        if (iter != m_managerHandles.end()) {
            strongIter = std::find_if(m_managers.begin(), m_managers.end(), [iter](const auto& i) {
                return iter->second == i;
            });
            if (strongIter != m_managers.end())
                m_managers.erase(strongIter);
        }
        
        strongIter = m_managers.insert(createManager<DerivedT>()).first;
        
        if (managerIter != m_managerHandles.end())
            managerIter->second = *strongIter;
        else
            m_managerHandles.emplace(getTypeId<ManagerT>(), *strongIter);
        if (derivedIter != m_managerHandles.end())
            managerIter->second = *strongIter;
        else
            m_managerHandles.emplace(getTypeId<DerivedT>(), *strongIter);
        
        return *strongIter;
    }

private:
    // Order of members is important as it affects order of destruction
    std::unordered_set<AnyStrongHandle> m_managers;
    std::unordered_map<TypeId, AnyHandle> m_managerHandles;
    StrongHandle<Entity> m_rootEntity;
    MemoryManager& m_memoryManager;

    template <typename T>
    static constexpr bool constructedWithHierarchy(decltype(T(std::declval<Handle<Hierarchy>>()))* = 0) { return true; }

    template <typename T>
    static constexpr bool constructedWithHierarchy(decltype(T())* = 0) { return false; }

    template <typename ManagerT>
    StrongHandle<ManagerT> createManager() {
        if constexpr (constructedWithHierarchy<ManagerT>())
            return m_memoryManager.create<ManagerT>(selfHandle());
        else
            return m_memoryManager.create<ManagerT>();
    }
};

} // flappy
