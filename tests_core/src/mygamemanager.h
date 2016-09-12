#pragma once

#include <core/manager.h>

namespace game {

class MyGameManager : public flappy::Manager
{
public:
    void init() override;
};

} // game
