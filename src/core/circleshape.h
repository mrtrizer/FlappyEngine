#pragma once

#include "rectshape.h"

namespace flappy {

/// Represents a circle shape.
class CircleShape  : public Presenter {
public:
    int id() override {return ClassId<Presenter,CircleShape>::id(); }
};

} // flappy
