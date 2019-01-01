#pragma once

#include <functional>
#include <list>
#include <queue>

#include <TypeId.hpp>
#include <Component.hpp>

namespace flappy {

class Hierarchy;

using DeltaTime = float;
    
class UpdateManager {
    friend class UpdateManagerDebugger;

    struct UpdateFunction {
        int id = 0;
        int depth = -1;
        std::function<void(DeltaTime dt)> updateFunction;
    };

    struct UpdateFunctionList {
        TypeId typeId;
        std::list<UpdateFunction> updateFunctions;
    };
public:
    UpdateManager(Handle<Hierarchy> hierarchy)
        : m_hierarchy(hierarchy)
    {}

    void update(DeltaTime dt);

    template <typename TypeT>
    std::vector<UpdateFunctionList>::iterator findTypeFunctionList() {
        return std::find_if(m_updateFunctionLists.begin(), m_updateFunctionLists.end(),
                     [typeId = getTypeId<TypeT>()] (const auto& item) {
                        return item.typeId == typeId;
                     });
    }

    template <typename TypeT>
    std::vector<UpdateFunctionList>::iterator initFunctionList() {
        if constexpr (std::is_base_of_v<PutAfterMarker, TypeT>) {
            auto dependencyIter = findTypeFunctionList<typename TypeT::PutAfterType>();
            if (dependencyIter == m_updateFunctionLists.end())
                dependencyIter = initFunctionList<typename TypeT::PutAfterType>();
            return m_updateFunctionLists.insert(std::next(dependencyIter), UpdateFunctionList{getTypeId<TypeT>()});
        } else {
            m_updateFunctionLists.emplace_back(UpdateFunctionList{getTypeId<TypeT>()});
            return std::prev(m_updateFunctionLists.end());
        }
    }

    template <typename TypeT>
    int registerUpdateFunction(int depth, const std::function<void(DeltaTime dt)>& updateFunction) {
        auto iter = findTypeFunctionList<TypeT>();
        if (iter == m_updateFunctionLists.end())
            iter = initFunctionList<TypeT>();
        auto destination = std::adjacent_find(iter->updateFunctions.begin(), iter->updateFunctions.end(),
            [depth](const auto& first, const auto& second) {
                return first.depth <= depth && depth < second.depth;
            });
        iter->updateFunctions.insert(destination, UpdateFunction{++m_idCounter, depth, updateFunction});
        return m_idCounter;
    }

    template <typename TypeT>
    void unregisterUpdateFunction(int id) {
        // FIXME: Could be optimized if remove only first found element
        auto iter = findTypeFunctionList<TypeT>();
        std::remove_if(iter->updateFunctions.begin(), iter->updateFunctions.end(),
                       [id](auto& item) { return item.id == id; });
    }

private:
    Handle<Hierarchy> m_hierarchy;
    std::vector<UpdateFunctionList> m_updateFunctionLists;
    int m_idCounter = 0;
};

}
