#ifndef GAMEMGR_H
#define GAMEMGR_H

#include <core/manager.h>

class GameMgr : public Manager<GameMgr>
{
public:
    GameMgr();
    virtual void init() = 0;
};

#endif // GAMEMGR_H
