#include <iostream>

#include "gworldmodel.h"

using namespace std;

GWorldModel::GWorldModel() {
}

void GWorldModel::initWorld() {
    gObj = std::make_shared<GObj>();
    lastTime = chrono::steady_clock::now();
    init();
}

void GWorldModel::run(const GContext & context) {
    auto newTime = chrono::steady_clock::now();
    float deltaT = chrono::duration <float, milli> (newTime - lastTime).count() / 1000.0f;
    lastTime = newTime;

    auto objects = getRoot()->findChilds();
    for (auto i : objects)
        i->recalc(deltaT, context);

    recalc(deltaT, context);
}
