#ifndef GOBJPOINTER_H
#define GOBJPOINTER_H

#include "gobj.h"
#include "gcollider.h"

/// Mouse pointer in a game world. It is created and managerd by
/// GObjCamera, because it knows screen parameters.
class GObjPointer: public GColliderCircle {
public:
    GObjPointer():
        GColliderCircle(0.1,GPos(0,0,0))
    {}
};

#endif // GOBJPOINTER_H
