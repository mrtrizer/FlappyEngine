#pragma once

#include <SDL2/SDL.h>

#include <IGLManager.h>

namespace flappy {

void checkSdlError(const char * file, const char * func, int line);

} // flappy

#define TRACE_SDL_ERRORS checkSdlError(__FILE__, __FUNCTION__, __LINE__)
