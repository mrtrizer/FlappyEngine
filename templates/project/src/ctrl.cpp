#include <core/gviewfactory.h>

#include "ctrl.h"

/// World initialization
/// @see setWorld()
void Ctrl::init() {
    setWorld(std::make_shared<World>());
}
