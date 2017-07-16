#pragma once

#include <string>

#include <ResManager.h>
#include <AtlasRes.h>
#include <TextureRes.h>
#include <ResFactory.h>

namespace flappy {

class Sdl2GlTextureResFactory :public ResFactory
{
public:
    Sdl2GlTextureResFactory(std::string path);
    std::shared_ptr<Res> load(const std::string&, SafePtr<ResManager>) override;

    class file_open_error {};

class wrong_bitmap_format {};
private:
    std::string m_path;
};

} // flappy