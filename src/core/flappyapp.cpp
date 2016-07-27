#include <core/inputmgr.h>

#include "flappyapp.h"

using namespace std;

FlappyApp::FlappyApp() {
}

void FlappyApp::update() {
    auto newTime = chrono::steady_clock::now();
    TimeDelta dt = chrono::duration <float, milli> (newTime - lastTime).count() / 1000.0f;
    lastTime = newTime;

    InputMgr::getInst()->update(dt);
    EntityMgr::getInst()->update(dt);
    worldView->update(dt);
}

void FlappyApp::configure() {
    init();
    configured = true;
    lastTime = chrono::steady_clock::now();
}
