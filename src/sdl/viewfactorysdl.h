#ifndef VIEW_FACTORY_SDL_H
#define VIEW_FACTORY_SDL_H

#include <string>

#include <gl/glviewfactory.h>

class GLTexture;

class ViewFactorySDL: public GLViewFactory {
public:
    ViewFactorySDL(std::string resPath);
    std::shared_ptr<GLTexture> getGLTexture(std::string path) const;

    ///Error on texture loading
    class loading_error {};

private:
    std::string resPath;
};

#endif //VIEW_FACTORY_SDL_H
