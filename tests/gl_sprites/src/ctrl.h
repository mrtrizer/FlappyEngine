#ifndef FLAPPYCTRL_H
#define FLAPPYCTRL_H

#include <memory>

#include <core/gworldctrl.h>
#include <core/gcontext.h>

#include "world.h"

class Ctrl: public GWorldCtrl {
public:
    void init();
};

#endif // FLAPPYCTRL_H
