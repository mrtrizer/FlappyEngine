#pragma once

#include <string>

#include <core/resmanager.h>
#include <core/atlas.h>
#include <core/texture.h>

namespace flappy {

class SDLTextureFactory :public IResFactory
{
public:
    SDLTextureFactory(std::string path);
    std::shared_ptr<IRes> load(const std::string& name) override;

    class file_open_error {};

class wrong_bitmap_format {};
private:
    std::string m_path;
};

} // flappy
