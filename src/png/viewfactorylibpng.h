#pragma once

#include <string>

#include <gl/glviewfactory.h>

namespace flappy {

class GLTexture;

class ViewFactoryLibPNG: public GLViewFactory {
public:
    ViewFactoryLibPNG(string resPath);
    shared_ptr<Texture> getTexture(string path) const override;

    class file_open_error {};
    /// Wrong file format (after checking png signature)
    class wrong_file_format {};
    /// Can't create libpng structures
    class out_of_memory {};
    class io_error {};
    class read_error {};
    /// An alpha chanel is missed. Save image with alpha chanel.
    class wrong_bitmap_format {};

private:
    string resPath;
};

} // flappy
