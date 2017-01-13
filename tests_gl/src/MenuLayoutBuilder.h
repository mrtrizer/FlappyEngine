#pragma once

#include <Component.h>

namespace flappy {

class MenuLayoutComponent: public Component
{
public:
    struct StartButtonPressed {};

    void init() override;
};

} // flappy
