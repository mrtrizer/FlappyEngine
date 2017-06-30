#include <SDL2/SDL_image.h>

#include "SdlGlTextureResFactory.h"
#include <GLTextureRes.h>

namespace flappy {

using namespace std;

/// @param resPath Path to resource dir
SdlGlTextureResFactory::SdlGlTextureResFactory(string path):
    m_path(path){

}

// http://www.libpng.org/pub/png/book/chapter13.html
/// @param path Relative path to image in resource dir without extension.
/// An image has to be saved with alpha chanel.
std::shared_ptr<Res> SdlGlTextureResFactory::load(const std::string& path, SafePtr<ResManager>) {
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

    auto result = make_shared<GLTexture>((char *)image->pixels, image->w, image->h);

    return move(result);
}

}
