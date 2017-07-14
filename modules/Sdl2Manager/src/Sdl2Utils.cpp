#include "Sdl2Utils.h"

namespace flappy {

void checkSdlError(const char * file, const char * func, int line)
{
    std::string error = SDL_GetError();

    if (error != "")
    {
        LOGE("SDL Error %s %s %d %s", error.c_str(), file, line, func);
        SDL_ClearError();
    }
}

} // flappy
