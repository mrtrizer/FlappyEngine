#include <SDL_image.h>

#include "Sdl2RgbaBitmapResFactory.h"
#include "Sdl2RgbaBitmapRes.h"

namespace flappy {

using namespace std;

/// @param resPath Path to resource dir
Sdl2RgbaBitmapResFactory::Sdl2RgbaBitmapResFactory(string path):
    m_path(path){

}

// http://www.libpng.org/pub/png/book/chapter13.html
/// @param path Relative path to image in resource dir without extension.
/// An image has to be saved with alpha chanel.
std::shared_ptr<Res> Sdl2RgbaBitmapResFactory::load(const std::string& path, SafePtr<ResManager>) {
    string fullPath = m_path + "/" + path + ".png";

    SDL_Surface* sdlSurfacePtr;
    if(!( sdlSurfacePtr = IMG_Load(fullPath.data()))) {
            throw file_open_error();
    }

    auto sdlSurface = std::unique_ptr<SDL_Surface>(sdlSurfacePtr);

    //TODO: Any other ways to switch R and B components?
    int count =  sdlSurfacePtr->w *  sdlSurfacePtr->h;
    char * pixels = static_cast<char *>( sdlSurfacePtr->pixels);
    for (int i = 0; i < count; i++) {
        char c = pixels[i * 4];
        pixels[i * 4] = pixels[i * 4 + 2];
        pixels[i * 4 + 2] = c;
    }

    auto result = make_shared<Sdl2RgbaBitmapRes>(std::move(sdlSurface));

    return result;
}

}
