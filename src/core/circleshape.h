#pragma once

#include "rectshape.h"

namespace flappy {

/// Represents a circle shape.
class CircleShape  : public RectShape {
public:
    using RectShape::RectShape;
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory& factory) override;
};

} // flappy
