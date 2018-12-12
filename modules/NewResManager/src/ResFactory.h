#pragma once

#include <memory>

#include <Manager.h>
#include <SafePtr.h>
#include <ResInfo.h>

#include "IResFactory.h"

namespace flappy {

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
template <typename ResT>
class ResFactory: public Manager<ResFactory<ResT>>, public IResFactory {

};

/// @}

} // flappy
