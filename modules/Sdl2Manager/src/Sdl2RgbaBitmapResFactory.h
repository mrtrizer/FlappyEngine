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
    std::shared_ptr<Res> load(const ResInfo&, SafePtr<Entity>) override;
    std::shared_ptr<Res> create(const std::string&, SafePtr<Entity>) override;

    class file_open_error {};

class wrong_bitmap_format {};
private:
    std::string m_path;
};

} // flappy
