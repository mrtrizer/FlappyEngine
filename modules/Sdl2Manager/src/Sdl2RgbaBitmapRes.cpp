#include "Sdl2RgbaBitmapRes.h"

namespace flappy {

Sdl2RgbaBitmapRes::Sdl2RgbaBitmapRes(std::unique_ptr<SDL_Surface> sdlSurface)
    :m_sdlSurface(std::move(sdlSurface))
{}

char* Sdl2RgbaBitmapRes::bitmapData()
{
    return (char*)m_sdlSurface->pixels;
}

Color Sdl2RgbaBitmapRes::pixelColor(int x, int y)
{
    return Color();
}

int Sdl2RgbaBitmapRes::width()
{
    return m_sdlSurface->w;
}

int Sdl2RgbaBitmapRes::height()
{
    return m_sdlSurface->h;
}


} // flappy

