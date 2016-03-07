#include "ganimation.h"

GAnimation::GAnimation(std::string path, float width, float height, GPos pos, int frameCnt, float period) :
    GPresenterSprite(path,width, height, pos, frameCnt),
    period(period) {

}

void GAnimation::recalc(GObj::DeltaT deltaT, const GContext &) {
    counter += deltaT;
    if (counter > period) {
        counter = 0;
        curFrame++;
        if (curFrame >= getFrameCnt())
            curFrame = 0;
        setFrameN(curFrame);
    }
}
