#ifndef GANIMATION_H
#define GANIMATION_H

#include <entityx/entityx.h>

#include "gpresenter.h"

/// Extends GDecor and allows automatic frame switching with predifined period.
class SpriteAnimation: public GPresenterSprite
{
public:
    /// @param period Period in seconds
    SpriteAnimation(std::string path, float width, float height, int frameCnt, float period);
    void update(entityx::TimeDelta) override;
private:
    float counter = 0.0;
    float period = 0.0;
    int curFrame = 0;
};

#endif // GANIMATION_H
