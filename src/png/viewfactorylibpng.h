#ifndef VIEW_FACTORY_LIB_PNG_H
#define VIEW_FACTORY_LIB_PNG_H

#include <string>

#include <gl/glviewfactory.h>

class GLTexture;

class ViewFactoryLibPNG: public GLViewFactory {
public:
    /// Path to resource dir
    ViewFactoryLibPNG(std::string resPath);
    /// Relative path to image in resource dir without extension
    /// An image has to be saved with alpha chanel
    std::shared_ptr<GLTexture> getGLTexture(std::string path) const;

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
    std::string resPath;
};

#endif //VIEW_FACTORY_LIB_PNG_H
