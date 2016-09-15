#pragma once

#include "presenter.h"
#include "color.h"

namespace flappy {

/// Represents a rectangle shape.
class RectShape : public Presenter {
    unsigned id() override {return ClassId<Presenter,RectShape>::id(); }
};

} // flappy
