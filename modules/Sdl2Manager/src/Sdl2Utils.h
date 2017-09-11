#pragma once

#include <SDL2/SDL.h>

#include <Manager.h>
#include <IGLManager.h>

namespace flappy {

void checkSdlError(const char * file, const char * func, int line);

} // flappy

#define CHECK_SDL_ERROR checkSdlError(__FILE__, __FUNCTION__, __LINE__)
