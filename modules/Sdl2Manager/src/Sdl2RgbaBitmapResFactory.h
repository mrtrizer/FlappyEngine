#pragma once

#include <string>

#include <ResFactory.h>
#include <IRgbaBitmapRes.h>

namespace flappy {

class Sdl2RgbaBitmapResFactory :public ResFactory<IRgbaBitmapRes>
{
public:
    Sdl2RgbaBitmapResFactory();
    std::shared_ptr<ResBase> load(const std::string&, ExecType) final;
    std::shared_ptr<ResBase> create(const std::string&) final;
    bool changed(const std::string&) final;
};

} // flappy
