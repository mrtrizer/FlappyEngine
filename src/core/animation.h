#pragma once

#include "sprite.h"

namespace flappy {

using namespace std;

class SpriteAnimation: public Sprite
{
public:
    // TODO: Make setter functions instead of cotr arguments
    /// @param period Period in seconds
    SpriteAnimation(const string& path, float width, float height, int frameCnt, float period);
    void update(TimeDelta) override;
private:
    float m_counter = 0.0;
    float m_period = 0.0;
    int m_curFrame = 0;
};

} // flappy
