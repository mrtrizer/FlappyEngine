#pragma once

#include "sprite.h"

namespace flappy {

class SpriteAnimation: public Sprite
{
public:
    // TODO: Make setter functions instead of cotr arguments
    /// @param period Period in seconds
    SpriteAnimation();
    void setPeriod(float period);
    void update(TimeDelta) override;
private:
    float m_counter = 0.0;
    float m_period = 0.0;
    int m_curFrame = 0;
};

} // flappy
