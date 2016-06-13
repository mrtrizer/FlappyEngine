#include "flappyapp.h"

#include <core/initsystem.h>

FlappyApp::FlappyApp() {
}

void FlappyApp::update(entityx::TimeDelta dt) {
    if (configured)
        systems.update_all(dt);
}

void FlappyApp::configure() {
    init();
    systems.add<InitSystem>();
    systems.configure();
    configured = true;
}
