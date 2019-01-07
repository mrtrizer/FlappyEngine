#pragma once

#include <memory>

#include <ResInfo.h>
#include <ReflectionMarkers.hpp>

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
class REFLECT ResFactory: public IResFactory {

};

/// @}

} // flappy
