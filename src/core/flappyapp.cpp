#include <core/initsystem.h>
#include <core/inputmanager.h>

#include "flappyapp.h"

using namespace std;

FlappyApp::FlappyApp() {
}

void FlappyApp::update() {
    auto newTime = chrono::steady_clock::now();
    entityx::TimeDelta dt = chrono::duration <float, milli> (newTime - lastTime).count() / 1000.0f;
    lastTime = newTime;

    InputManager::getInst()->update(dt);

    if (configured)
        systems.update_all(dt);
}

void FlappyApp::configure() {
    init();
    systems.add<InitSystem>();
    systems.configure();
    configured = true;
    lastTime = chrono::steady_clock::now();
}
