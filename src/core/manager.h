#pragma once

#include <memory>

#include "tools.h"

template <typename Derived>
class Manager
{
public:
    Manager() {}

    Manager(const Manager&) = delete;
    void operator=(Manager const&) = delete;

    virtual void update(TimeDelta){}
};
