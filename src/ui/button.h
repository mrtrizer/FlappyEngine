#pragma once

#include <core/entitymgr.h>

namespace flappy {

class Button: public Component {
public:
    void update(TimeDelta dt);

    function<void()> onClick;

private:
    bool isInField();
};

} // flappy
