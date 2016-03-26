#include <cstdlib>

#include "world.h"
#include "ctrl.h"

World::World() {
}

void World::recalc(GObj::DeltaT, const GContext & gContext) {

}

void World::init() {
    sprite = getRoot()->ADD_CHILD(GAnimation,"bird",10,10,POS(-5,-5,1),2,0.5);
    getRoot()->ADD_CHILD(GPresenterSprite,"background",200,200,POS(-100,-100,0));
    setActiveCamera(getRoot()->ADD_CHILD(GObjCamera,100,1.0,500,POS(0,0,0)));
}
