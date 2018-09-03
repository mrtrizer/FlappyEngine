#pragma once

#include "Function.hpp"

namespace flappy {

struct Constructor {
    Function onStackConstructor;
    Function onHeapConstructor;
    Function inAddressConstructor;
};

} // flappy
