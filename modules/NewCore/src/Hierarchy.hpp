#pragma once

#include <array>
#include "ObjectPool.hpp"
#include "Entity.hpp"

namespace flappy {

class Entity;

class Hierarchy : public EnableSelfHandle<Hierarchy> {
public:
    Hierarchy()
        : m_rootEntity(m_entityPool.create<Entity>(selfHandle()))
    {}

    ~Hierarchy() {}

    template <typename ManagerT>
    Handle<ManagerT> manager() {
        return static_cast<StrongHandle<ManagerT>*>(&m_managers.at(getTypeId<ManagerT>()))->handle();
    }

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        size_t size = sizeof(DataT);
        for (auto& objectPool : m_objectPools) {
            if (objectPool.maxObjectSize() >= size)
                return objectPool.create<DataT>(std::forward<Args>(args)...);
        }
        throw std::runtime_error(sstr("Can't find appropriate object pool for object of size ", size));
    }

    Handle<Entity> rootEntity() {
        return m_rootEntity;
    }

    template <typename ManagerT, typename DerivedT = ManagerT>
    Handle<ManagerT> initManager() {
        auto manager = createManager<DerivedT>();
        auto managerHandle = manager.handle();
        auto iter = m_managers.find(getTypeId<ManagerT>());
        if (iter != m_managers.end())
            iter->second = std::move(manager);
        else
            m_managers.emplace(getTypeId<ManagerT>(), std::move(manager));
        return managerHandle;
    }

private:
    // Order of members is important as it affects order of destruction. m_rootEntity should be the last member.
    std::unordered_map<TypeId, AnyStrongHandle> m_managers;
    ObjectPool m_entityPool { sizeof(Entity), 1000 };
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 100), ObjectPool(256, 50), ObjectPool(1024, 20) };
    StrongHandle<Entity> m_rootEntity;

    template <typename T>
    static constexpr bool constructedWithHierarchy(decltype(T(std::declval<Handle<Hierarchy>>()))* = 0) { return true; }

    template <typename T>
    static constexpr bool constructedWithHierarchy(decltype(T())* = 0) { return false; }

    template <typename ManagerT>
    StrongHandle<ManagerT> createManager() {
        if constexpr (constructedWithHierarchy<ManagerT>())
            return create<ManagerT>(selfHandle());
        else
            return create<ManagerT>();
    }
};

} // flappy
