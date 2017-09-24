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
    std::shared_ptr<ResBase> load(const std::string&) final;
    std::shared_ptr<ResBase> create(const std::string&) final;
    bool changed(const std::string&) final;

private:
    std::string m_path;
};

} // flappy
