#ifndef FLAPPYAPP_H
#define FLAPPYAPP_H

#include <chrono>
#include <entityx/entityx.h>
#include <core/scenemanager.h>

class FlappyApp : public entityx::EntityX {
public:
    explicit FlappyApp();
    void update();
    virtual void init() = 0;
    void configure();

private:
    bool configured = false;
    std::chrono::steady_clock::time_point lastTime;
};

#endif // FLAPPYAPP_H
