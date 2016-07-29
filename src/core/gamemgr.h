#pragma once

#include <core/manager.h>

class GameMgr : public Manager<GameMgr>
{
public:
    GameMgr();
    virtual void init() = 0;
};
