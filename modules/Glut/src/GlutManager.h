#pragma once

#include <Manager.h>

namespace flappy {

class GlutManager : public Manager
{
public:
    GlutManager();

protected:
    void init() override;
};

} // flappy
