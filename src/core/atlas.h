#pragma once

#include "tools.h"

namespace flappy {

class Atlas
{
public:
    Atlas();
    Tools::Rect getRect(std::string name);
};

} // flappy
