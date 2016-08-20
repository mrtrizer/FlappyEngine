#pragma once

#include <string>

#include <core/iresourceloader.h>
#include <core/atlas.h>
#include <core/texture.h>

namespace flappy {

class Texture;

class LibPNGResourceLoader :public IResourceLoader
{
public:
    LibPNGResourceLoader(std::string path);
    virtual std::unique_ptr<Texture> getTexture(const std::string& name) const override;
    virtual std::unique_ptr<Atlas> getAtlas(const std::string& name) const override {return nullptr;}

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
    std::string m_path;
};

} // flappy

