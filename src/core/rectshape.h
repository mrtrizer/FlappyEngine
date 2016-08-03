#pragma once

#include "presenter.h"

namespace flappy {

using namespace glm;

/// Represents a rectangle shape.
class RectShape : public Presenter {
public:
    RectShape(float width, float height):
        m_width(width),
        m_height(height)
    {}
    virtual vec3 size() {return vec3(m_width,m_height,0);}
    virtual ~RectShape(){}
    float width() const { return m_width; }
    float height() const { return m_height; }
protected:
    virtual shared_ptr<View> makeGView(const ViewFactory& factory) override;
private:
    float m_width;
    float m_height;
};

} // flappy
