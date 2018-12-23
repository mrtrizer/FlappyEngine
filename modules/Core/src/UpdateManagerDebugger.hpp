#pragma once

#include <string>

#include <Handle.hpp>

#include "UpdateManager.hpp"

namespace flappy {

#ifndef NDEBUG
class UpdateManagerDebugger {
public:
    static std::string componentOrder(const Handle<UpdateManager>& updateManager) {
        std::stringstream updateFunctionsSS;
        for (auto updateFunctionList : updateManager->m_updateFunctionLists)
            for (auto updateFunction : updateFunctionList.updateFunctions)
                updateFunctionsSS << updateFunction.depth;
        return updateFunctionsSS.str();
    }
    static std::string componentOrderFull(const Handle<UpdateManager>& updateManager) {
        std::stringstream updateFunctionsSS;
        for (auto updateFunctionList : updateManager->m_updateFunctionLists)
            for (auto updateFunction : updateFunctionList.updateFunctions)
                updateFunctionsSS << getTypeName(updateFunctionList.typeId) << " : " << updateFunction.depth << std::endl;
        return updateFunctionsSS.str();
    }
};
#endif

} // flappy
