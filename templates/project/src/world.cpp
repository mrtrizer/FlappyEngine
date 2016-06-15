#include <cstdlib>

#include "world.h"
#include "ctrl.h"

World::World() {
}

void World::recalc(GObj::DeltaT, const GContext &) {

}

void World::init() {
    sprite = getRoot()->ADD_CHILD(GAnimation,"bird",10,10,GPos(-5,-5,1),2,0.5);
    getRoot()->ADD_CHILD(GPresenterSprite,"background",200,200,GPos(-100,-100,0));
    setActiveCamera(getRoot()->ADD_CHILD(GObjCamera,100,1.0,500,GPos(0,0,0)));
}
