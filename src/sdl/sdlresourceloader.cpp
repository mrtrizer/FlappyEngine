#include <SDL/SDL_image.h>

#include "sdlresourceloader.h"
#include <gl/gltexture.h>

namespace flappy {

using namespace std;

/// @param resPath Path to resource dir
SDLResourceLoader::SDLResourceLoader(string path):
    m_path(path){

}

// http://www.libpng.org/pub/png/book/chapter13.html
/// @param path Relative path to image in resource dir without extension.
/// An image has to be saved with alpha chanel.
unique_ptr<Texture> SDLResourceLoader::getTexture(const string& path) const {
    string fullPath = m_path + "/" + path + ".png";

    SDL_Surface *image;
    if(!(image = IMG_Load(fullPath.data()))) {
        throw file_open_error();
    }

    //TODO: Any other ways to switch R and B components?
    int count = image->w * image->h;
    char * pixels = static_cast<char *>(image->pixels);
    for (int i = 0; i < count; i++) {
        char c = pixels[i * 4];
        pixels[i * 4] = pixels[i * 4 + 2];
        pixels[i * 4 + 2] = c;
    }

    auto result = unique_ptr<GLTexture>(new GLTexture((char *)image->pixels, image->w, image->h));

    return move(result);
}

}
