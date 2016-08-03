#pragma once

#include <core/manager.h>

namespace flappy {

class GameMgr : public Manager<GameMgr>
{
public:
    GameMgr();
    virtual void init() = 0;
};

} // flappy
