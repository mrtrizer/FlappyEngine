#pragma once

#include <string>

#include <gl/glviewfactory.h>

namespace flappy {

using namespace std;

class GLTexture;

class ViewFactorySDL: public GLViewFactory {
public:
    ViewFactorySDL(string resPath);
    shared_ptr<Texture> getTexture(string path) const override;

    ///Error on texture loading
    class loading_error {};

private:
    string resPath;
};

} // flappy
