#ifndef FLAPPYAPP_H
#define FLAPPYAPP_H

#include <entityx/entityx.h>

class FlappyApp : public entityx::EntityX {
public:
    explicit FlappyApp();
    void update(entityx::TimeDelta dt);
    virtual void init() = 0;
    void configure();
private:
    bool configured = false;
};

#endif // FLAPPYAPP_H
