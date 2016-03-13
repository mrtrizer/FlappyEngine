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

    auto result = std::make_shared<GLTexture>((char *)image->pixels, image->w, image->h);

    return result;
}
