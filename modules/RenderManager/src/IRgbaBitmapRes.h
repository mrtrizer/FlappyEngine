#pragma once

#include <unordered_map>

#include <Tools.h>
#include <Res.h>
#include <Color.h>

namespace flappy {

class IRgbaBitmapRes: public Res
{
public:
    std::list<std::shared_ptr<Res>> dependencyList() const override { return {}; }

    virtual char* bitmapData() = 0;
    virtual Color pixelColor(int x, int y) = 0;
    virtual int width() = 0;
    virtual int height() = 0;
};

} // flappy
