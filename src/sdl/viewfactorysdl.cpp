#include <SDL/SDL_image.h>

#include "viewfactorysdl.h"
#include <gl/gltexture.h>

/// @param resPath Path to resource dir
ViewFactorySDL::ViewFactorySDL(std::string resPath):
    resPath(resPath){

}

// http://www.libpng.org/pub/png/book/chapter13.html
/// @param path Relative path to image in resource dir without extension.
/// An image has to be saved with alpha chanel.
std::shared_ptr<GLTexture> ViewFactorySDL::getGLTexture(std::string path) const {
    std::string fullPath = resPath + "/" + path + ".png";

    SDL_Surface *image;
    if(!(image = IMG_Load(fullPath.data()))) {
        throw loading_error();
    }

    //TODO: Any other ways to switch R and B components?
    int count = image->w * image->h;
    char * pixels = static_cast<char *>(image->pixels);
    for (int i = 0; i < count; i++) {
        char c = pixels[i * 4];
        pixels[i * 4] = pixels[i * 4 + 2];
        pixels[i * 4 + 2] = c;
    }

    auto result = std::make_shared<GLTexture>((char *)image->pixels, image->w, image->h);

    return result;
}
