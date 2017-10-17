#pragma once

#include <SDL2/SDL.h>

#include <Manager.h>

namespace flappy {

class Sdl2KeyboardInput : public Manager<Sdl2KeyboardInput>
{
public:
    Sdl2KeyboardInput();
};

} // flappy
