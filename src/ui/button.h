#ifndef BUTTON_H
#define BUTTON_H

#include <core/entitymanager.h>

class Button: public Component {
public:
    void update(TimeDelta dt);

    std::function<void()> onClick;

private:
    bool isInField();
};

#endif // BUTTON_H
