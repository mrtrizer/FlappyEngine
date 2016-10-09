#pragma once

#include <core/manager.h>

namespace flappy {
class AppManager;
}

namespace game {

class MyGameManager : public flappy::Manager<flappy::AppManager>
{
public:
    void init() override;
};

} // game
