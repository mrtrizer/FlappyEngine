#ifndef AGLMANAGER_H
#define AGLMANAGER_H

#include <Manager.h>

namespace flappy {

/// Base class for all GL managers like SDL, GLUT and so on
class IGLManager : public Manager<IGLManager> {
public:
    using Manager::Manager;

    virtual int startMainLoop() = 0;
};

}

#endif // AGLMANAGER_H
