#ifndef AGLMANAGER_H
#define AGLMANAGER_H

namespace flappy {

/// Base class for all GL managers like SDL, GLUT and so on
class REFLECT IGLManager {
public:
    virtual ~IGLManager() = default;
};

}

#endif // AGLMANAGER_H
