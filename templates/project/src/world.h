#ifndef FLAPPYWORLD_H
#define FLAPPYWORLD_H

#include <core/gworldmodel.h>
#include <core/ganimation.h>

class Ctrl;
class Score;

class World: public GWorldModel
{
public:
    World();

protected:
    void recalc(GObj::DeltaT, const GContext &) override;
    virtual void init() override;

private:
    std::shared_ptr<GAnimation> sprite;
};

#endif // FLAPPYWORLD_H
