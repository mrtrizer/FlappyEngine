#pragma once

#include <string>

#include <ResManager.h>
#include <AtlasRes.h>
#include <TextureRes.h>
#include <IResFactory.h>
#include <IRgbaBitmapRes.h>

namespace flappy {

class Sdl2RgbaBitmapResFactory :public ResFactory<IRgbaBitmapRes>
{
public:
    Sdl2RgbaBitmapResFactory();
    std::shared_ptr<Res> load(const std::string&) override;
    std::shared_ptr<Res> create(const std::string&) override;

private:
    std::string m_path;
};

} // flappy
