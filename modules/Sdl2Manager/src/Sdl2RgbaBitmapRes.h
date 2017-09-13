#pragma once

#include <SDL_image.h>

#include <IRgbaBitmapRes.h>

namespace flappy {

class Sdl2RgbaBitmapRes: public IRgbaBitmapRes
{
public:
    Sdl2RgbaBitmapRes(std::unique_ptr<SDL_Surface> sdlSurface);

    char* bitmapData() final;
    Color pixelColor(int x, int y) final;
    int width() final;
    int height() final;

private:
    std::unique_ptr<SDL_Surface> m_sdlSurface;
};

} // flappy
