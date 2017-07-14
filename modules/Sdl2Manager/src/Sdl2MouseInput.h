#pragma once

#include <SDL2/SDL.h>

#include <Manager.h>

namespace flappy {

class Sdl2MouseInput : public Manager<Sdl2MouseInput>
{
public:
    Sdl2MouseInput();
};

} // flappy
