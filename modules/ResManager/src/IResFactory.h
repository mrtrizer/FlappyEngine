#pragma once

#include <memory>

#include <Manager.h>
#include <SafePtr.h>
#include <ResInfo.h>

namespace flappy {

class Res;
class Entity;

/// @addtogroup ResManager
/// @{

class IResFactory {
public:
    virtual std::shared_ptr<Res> load(const std::string& resInfo) = 0;
    virtual std::shared_ptr<Res> create(const std::string& name) = 0;
};

/// Base class for all resource factories.
/// @details Every automaticly loaded resource should have
/// factory. Factories are used to create resource instacies
/// different ways on different platforms.
/// Factory has create() method to create default resource
/// instace right on first user request.
/// load() method should provide synchronious loading of
/// requested resource. It's usially called in a separate thread.
template <typename ResT>
class ResFactory: public Manager<ResFactory<ResT>>, public IResFactory {

};

/// @}

} // flappy
