#pragma once

#include "rectshape.h"

namespace flappy {

using namespace std;

/// Represents a circle shape.
class CircleShape  : public RectShape {
public:
    using RectShape::RectShape;
protected:
    virtual shared_ptr<View> makeGView(const ViewFactory& factory) override;
};

} // flappy
