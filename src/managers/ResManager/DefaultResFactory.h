#pragma once

#include "ResFactory.h"

namespace flappy {

/// Pass dependent classes in DependT argument.
template <typename ResT, typename ... DependT>
class DefaultResFactory : public ResFactory
{
public:
    DefaultResFactory() = default;

    virtual std::shared_ptr<Res> load(const std::string& name, std::shared_ptr<ResManager> resManager) {
        return std::make_shared<ResT>(resManager->getRes<DependT>(name) ...);
    }
};

} // flappy
