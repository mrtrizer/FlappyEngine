#pragma once

#include <functional>

#include <SafePtr.h>
#include <Entity.h>

namespace flappy {

class Thread {
public:
    virtual int run(std::function<void(SafePtr<Entity>)> initCallback) = 0;
};

} //flappy
