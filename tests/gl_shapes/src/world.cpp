#include <cstdlib>

#include "world.h"
#include "ctrl.h"

World::World() {
}

void World::recalc(GObj::DeltaT delta, const GContext &) {
    rect->setPos(rect->getPos() + GPos(1.0 * delta,0));
}

void World::init() {
    rect = getRoot()->ADD_CHILD(GPresenterRect, 20, 20, GPos(-10,-10));
    setActiveCamera(getRoot()->ADD_CHILD(GObjCamera,100,1.0,500,GPos{0,0,0}));
}
