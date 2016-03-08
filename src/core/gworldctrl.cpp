#include "gworldctrl.h"
#include "gworldmodel.h"
#include "gworldview.h"
#include "gviewfactory.h"

/// Switch current world. It's used to switch between FlappyMenu and FlappyWorld
void GWorldCtrl::setWorld(const GWorldModelP &gWorld) {
    curWorld = gWorld;
    curWorld->initWorld();
    if (gWorldView != nullptr)
        setView(gWorldView);
}

/// Mouse click event
void GWorldCtrl::mouseClick(int x, int y) {
    events.push(GContext(x,y,GContext::CLICK));
}

/// Mouse move event
void GWorldCtrl::mouseMove(int x, int y) {
    events.push(GContext(x,y,GContext::EMPTY));
}

/// Call a game loop step
void GWorldCtrl::step() {
    //if events not empty, repeat a game loop iteration
    //for every event. else, run once with empty event.
    if (events.size() == 0) {
        curWorld->run(GContext(0,0,GContext::EMPTY));
        procState();
    } else {
        while (events.size() > 0) {
            curWorld->run(events.front());
            events.pop();
            //check symbols after iteration
            procState();
        }
    }
}

void GWorldCtrl::setView(const GWorldViewP &gWorldView){
    this->gWorldView = gWorldView;
    this->gWorldView->setGWorldModel(curWorld);
}
