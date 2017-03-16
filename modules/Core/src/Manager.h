#pragma once

#include <memory>

#include "Tools.h"
#include "Component.h"

namespace flappy {

class Manager: public Component {
public:
    struct OnManagerAdded: IEvent {
        unsigned id;
        SafePtr<Manager> pointer;
    };

    struct OnManagerRemoved: IEvent {
        unsigned id;
        SafePtr<Manager> pointer;
    };

private:
    void initInternal() override;
    void deinitInternal() override;

};

} // flappy
