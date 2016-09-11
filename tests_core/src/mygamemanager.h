#pragma once

#include <core/gamemanager.h>

namespace game {

class MyGameManager : public flappy::GameManager
{
public:
    void init() override;
};

} // game
