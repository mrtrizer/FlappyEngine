#pragma once

#include "presenter.h"
#include "color.h"

namespace flappy {

/// Represents a rectangle shape.
class RectShape : public Presenter {
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory& factory) override;
};

} // flappy
