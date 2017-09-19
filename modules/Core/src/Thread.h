#pragma once

#include <functional>

#include <SafePtr.h>
#include <Entity.h>

namespace flappy {

class Thread {
public:
    virtual int run() = 0;
};

} //flappy
