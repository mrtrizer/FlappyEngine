#pragma once

#include <string>

#include <core/resmanager.h>
#include <core/atlas.h>
#include <core/texture.h>

namespace flappy {

class Texture;

class LibPNGTextureFactory :public IResFactory
{
public:
    LibPNGTextureFactory(std::string path);
    std::shared_ptr<IRes> load(const std::string& path) override;

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
