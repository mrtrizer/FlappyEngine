#include "UpdateManager.hpp"

#include "Hierarchy.hpp"

namespace flappy {

void UpdateManager::update(DeltaTime dt) {
    for (const auto& updateFunctionList : m_updateFunctionLists) {
        for (const auto& updateFunction : updateFunctionList.updateFunctions)
            if (updateFunction.depth != -1)
                updateFunction.updateFunction(dt);
    }
}

}
