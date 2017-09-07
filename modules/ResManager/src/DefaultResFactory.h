#pragma once

#include <Entity.h>

#include "IResFactory.h"
#include "ResManager.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

/// Base class for simple wrapper resource factories.
/// @details This factory just creates an instance of a resource and
/// updates it if dependencies was updated.
/// Pass dependent classes in DependT argument.
template <typename BaseResT, typename ResT, typename ... DependT>
class DefaultResFactory : public ResFactory<BaseResT>
{
public:

    // Recursive adding of dependencies listed in DependT variadic
    struct Empty {};

    // for empty dependencies
    template <typename EmptyT>
    void addDependencies() {
    }

    // for the rest dependencies
    template <typename EmptyT, typename FirstDependT, typename ... RestDependT>
    void addDependencies() {
        this->template addDependency(ResManager<FirstDependT>::id());
        addDependencies<Empty, RestDependT...>();
    }

    DefaultResFactory() {
        addDependencies<Empty, DependT...>();
    }

    virtual std::shared_ptr<Res> load(const std::string& name) final {
        return create(name);
    }

    virtual std::shared_ptr<Res> create(const std::string& name) final {
        return std::make_shared<ResT>(this->template manager<ResManager<DependT>>()->template getRes(name) ...);
    }
};

/// @}

} // flappy
