#pragma once

#include <core/gamemgr.h>

namespace game {

class MyGameMgr : public flappy::GameMgr
{
public:
    void init() override;
};

} // game
