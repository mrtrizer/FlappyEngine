#pragma once

#include "IResFactory.h"
#include "ResManager.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

/// Base class for simple wrapper resource factories.
/// @details This factory just creates an instance of a resource and
/// updates it if dependencies was updated.
/// Pass dependent classes in DependT argument.
template <typename ResT, typename ... DependT>
class DefaultResFactory : public IResFactory
{
public:
    DefaultResFactory() = default;

    virtual std::shared_ptr<Res> load(const std::string& name, SafePtr<ResManager> resManager) {
        return std::make_shared<ResT>(resManager->getRes<DependT>(name) ...);
    }
};

/// @}

} // flappy
