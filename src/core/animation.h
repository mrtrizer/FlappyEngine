#ifndef GANIMATION_H
#define GANIMATION_H

#include "presenter.h"

class SpriteAnimation: public Sprite
{
public:
    /// @param period Period in seconds
    SpriteAnimation(std::string path, float width, float height, int frameCnt, float period);
    void update(TimeDelta) override;
private:
    float counter = 0.0;
    float period = 0.0;
    int curFrame = 0;
};

#endif // GANIMATION_H
