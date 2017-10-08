#include "Sdl2RgbaBitmapResFactory.h"

#include <SDL_image.h>

#include <ResManager.h>
#include <AtlasRes.h>
#include <TextureRes.h>
#include <ResRepositoryManager.h>

#include "Sdl2RgbaBitmapRes.h"

namespace flappy {

using namespace std;

Sdl2RgbaBitmapResFactory::Sdl2RgbaBitmapResFactory() {
    addDependency(ResRepositoryManager::id());
    addDependency(IFileMonitorManager::id());
}

// http://www.libpng.org/pub/png/book/chapter13.html
/// @param path Relative path to image in resource dir without extension.
/// An image has to be saved with alpha chanel.
std::shared_ptr<ResBase> Sdl2RgbaBitmapResFactory::load(const std::string& name, ExecType) {
    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    string fullPath = fileInfo.path;
    manager<IFileMonitorManager>()->registerFile(fullPath);

    SDL_Surface* sdlSurfacePtr;
    if(!( sdlSurfacePtr = IMG_Load(fullPath.data()))) {
        throw std::runtime_error("File open error");
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

std::shared_ptr<ResBase> Sdl2RgbaBitmapResFactory::create(const std::string&) {
    SDL_Surface* sdlSurfacePtr = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    auto sdlSurface = std::unique_ptr<SDL_Surface>(sdlSurfacePtr);
    SDL_FillRect(sdlSurfacePtr, NULL, SDL_MapRGB(sdlSurfacePtr->format, 255, 0, 0));

    auto result = make_shared<Sdl2RgbaBitmapRes>(std::move(sdlSurface));

    return result;
}

bool Sdl2RgbaBitmapResFactory::changed(const string & name) {
    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    return manager<IFileMonitorManager>()->changed(fileInfo.path);
}

}
