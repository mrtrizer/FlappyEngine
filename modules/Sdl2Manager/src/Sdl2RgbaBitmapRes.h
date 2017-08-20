#pragma once

#include <SDL_image.h>

#include <IRgbaBitmapRes.h>

namespace flappy {

class Sdl2RgbaBitmapRes: public IRgbaBitmapRes
{
public:
    Sdl2RgbaBitmapRes(std::unique_ptr<SDL_Surface> sdlSurface);

    std::list<std::shared_ptr<Res>> dependencyList() const override { return {}; }

    char* bitmapData() override;
    Color pixelColor(int x, int y) override;
    int width() override;
    int height() override;

private:
    std::unique_ptr<SDL_Surface> m_sdlSurface;
};

} // flappy
