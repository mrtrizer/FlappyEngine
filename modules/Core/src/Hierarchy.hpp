#pragma once

#include <array>
#include "ObjectPool.hpp"
#include "Entity.hpp"

namespace flappy {

class Entity;

class Hierarchy : public EnableSelfHandle<Hierarchy> {
public:
    Hierarchy()
        : m_rootEntity(m_entityPool.create<Entity>(nullptr, selfHandle()))
    {}

    ~Hierarchy() = default;

    template <typename ManagerT>
    Handle<ManagerT> manager() {
        auto typeId = getTypeId<ManagerT>();
        try {
            return static_cast<StrongHandle<ManagerT>*>(&m_managers.at(typeId))->handle();
        } catch (const std::exception&) {
            throw FlappyException("Can't find manager " + typeId.name);
        }
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

    template <typename ManagerT, typename DerivedT = ManagerT, typename ... ArgT>
    Handle<DerivedT> initManager(ArgT&& ... args) {
        static_assert(!std::is_abstract<DerivedT>(), "Can't construct manager of abstract type.");
        auto manager = createManager<DerivedT>(std::forward<ArgT>(args)...);
        auto managerHandle = manager.handle();
        auto iter = m_managers.find(getTypeId<ManagerT>());
        if (iter != m_managers.end())
            iter->second = std::move(manager);
        else
            m_managers.emplace(getTypeId<ManagerT>(), std::move(manager));
        return managerHandle;
    }

private:
    // Order of members is important as it affects order of destruction
    std::unordered_map<TypeId, AnyStrongHandle> m_managers;
    ObjectPool m_entityPool { sizeof(Entity), 1000 };
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 2000), ObjectPool(256, 1500), ObjectPool(1024, 200) };
    StrongHandle<Entity> m_rootEntity;

    template <typename T, typename ... ArgT>
    static constexpr bool constructedWithHierarchy(decltype(T(std::declval<Handle<Hierarchy>>()))* = 0) { return true; }

    template <typename T, typename ... ArgT>
    static constexpr bool constructedWithHierarchy(decltype(T())* = 0) { return false; }

    template <typename ManagerT, typename ... ArgT>
    StrongHandle<ManagerT> createManager(ArgT&& ... args) {
        if constexpr (constructedWithHierarchy<ManagerT, ArgT...>())
            return create<ManagerT>(selfHandle(), std::forward<ArgT>(args)...);
        else
            return create<ManagerT>(std::forward<ArgT>(args)...);
    }
};

} // flappy
