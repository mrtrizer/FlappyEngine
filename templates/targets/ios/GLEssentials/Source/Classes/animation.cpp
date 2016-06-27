#include "animation.h"

SpriteAnimation::SpriteAnimation(std::string path, float width, float height, int frameCnt, float period) :
    GPresenterSprite(path,width, height, frameCnt),
    period(period) {

}

void SpriteAnimation::update(entityx::TimeDelta dt) {
    counter += dt;
    if (counter > period) {
        counter = 0;
        curFrame++;
        if (curFrame >= getFrameCnt())
            curFrame = 0;
        setFrameN(curFrame);
    }
}
