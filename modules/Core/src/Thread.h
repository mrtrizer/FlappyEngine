#pragma once

#include <SafePtr.h>

namespace flappy {

class Entity;

class Thread {
public:

    virtual int run(SafePtr<Entity> entity) = 0;
};

} //flappy
