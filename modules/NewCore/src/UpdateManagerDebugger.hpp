#pragma once

#include <string>

#include <Handle.hpp>

#include "UpdataManager.hpp"

namespace flappy {

class UpdateManagerDebugger {
public:
    static std::string componentOrder(const Handle<UpdateManager>& updateManager) {
        std::stringstream updateFunctionsSS;
        for (auto updateFunction : updateManager->m_updateFunctions)
            updateFunctionsSS << updateFunction.depth;
        return updateFunctionsSS.str();
    }
};

} // flappy
