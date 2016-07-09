#ifndef FLAPPYAPP_H
#define FLAPPYAPP_H

#include <chrono>
#include <core/entitymanager.h>
#include <core/scene.h>
#include <core/viewmanager.h>

class FlappyApp {
public:
    explicit FlappyApp();
    void update();
    virtual void init() = 0;
    void configure();
    void setWorldView(std::shared_ptr<ViewManager> worldView) {
        this->worldView = worldView;
    }

private:
    bool configured = false;
    std::chrono::steady_clock::time_point lastTime;
    std::shared_ptr<ViewManager> worldView;
};

#endif // FLAPPYAPP_H
