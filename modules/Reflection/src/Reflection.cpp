#include "TypeId.hpp"
#include "Type.hpp"

namespace flappy {

// FIXME: I would prefere to make reflection library header only. But, Type.hpp and Reflection.hpp have
// cross references. I could put both Type and Reflection types to the one file for example, this would be
// the best solution I think.

std::shared_ptr<Type> Reflection::createType(const TypeId& typeId) {
    return std::make_shared<Type>(typeId);
}

} // flappy
