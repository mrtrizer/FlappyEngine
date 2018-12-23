#pragma once

#include <unordered_map>

#include <Res.h>
#include <Color.h>

namespace flappy {

class IRgbaBitmapRes: public Res<IRgbaBitmapRes>
{
public:
    virtual char* bitmapData() = 0;
    virtual Color pixelColor(int x, int y) = 0;
    virtual int width() = 0;
    virtual int height() = 0;
};

} // flappy
