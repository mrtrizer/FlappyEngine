#include "UpdateManager.hpp"

#include "Hierarchy.hpp"

namespace flappy {

void UpdateManager::update(DeltaTime dt) {
    
    for (auto& updateFunctionList : m_updateFunctionLists) {
        updateFunctionList.updateFunctions.remove_if([](auto& item) { return item.depth == -1; });
        for (const auto& updateFunction : updateFunctionList.updateFunctions)
            if (updateFunction.depth != -1)
                updateFunction.updateFunction(dt);
    }
}

}
