#pragma once

#include "presenter.h"

namespace flappy {

/// Represents a rectangle shape.
class RectShape : public Presenter {
public:
    virtual ~RectShape(){}

protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory& factory) override;
};

} // flappy
