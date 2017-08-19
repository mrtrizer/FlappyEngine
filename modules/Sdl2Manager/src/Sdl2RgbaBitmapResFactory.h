#pragma once

#include <string>

#include <ResManager.h>
#include <AtlasRes.h>
#include <TextureRes.h>
#include <IResFactory.h>

namespace flappy {

class Sdl2RgbaBitmapResFactory :public IResFactory
{
public:
    Sdl2RgbaBitmapResFactory(std::string path);
    std::shared_ptr<Res> load(const std::string&, SafePtr<ResManager>) override;

    class file_open_error {};

class wrong_bitmap_format {};
private:
    std::string m_path;
};

} // flappy
