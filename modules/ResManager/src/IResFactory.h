#pragma once

#include <memory>

#include <SafePtr.h>
#include <ResInfo.h>

namespace flappy {

class Res;
class Entity;

/// @addtogroup ResManager
/// @{

/// Base class for all resource factories.
/// @details Every automaticly loaded resource should have
/// factory. Factories are used to create resource instacies
/// different ways on different platforms.
/// Factory has create() method to create default resource
/// instace right on first user request.
/// load() method should provide synchronious loading of
/// requested resource. It's usially called in a separate thread.
/// changed method returns true if resource was changed since
/// last load.
class IResFactory {
public:
    IResFactory() = default;
    virtual ~IResFactory() = default;
    IResFactory(const IResFactory&) = default;
    IResFactory& operator=(const IResFactory&) & = default;
    IResFactory(IResFactory&&) = default;
    IResFactory& operator=(IResFactory&&) & = default;

    virtual std::shared_ptr<Res> load(const ResInfo& resInfo, SafePtr<Entity>) = 0;
    virtual std::shared_ptr<Res> create(const std::string& name, SafePtr<Entity>) = 0;
};

/// @}

} // flappy
