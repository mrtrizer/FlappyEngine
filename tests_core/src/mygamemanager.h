#pragma once

#include <core/manager.h>

namespace game {

class MyGameManager : public flappy::Manager<MyGameManager>
{
public:
    void init() override;
};

} // game
