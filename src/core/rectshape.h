#pragma once

#include "presenter.h"

/// Represents a rectangle shape.
class RectShape : public Presenter {
public:
    RectShape(float width, float height):
        m_width(width),
        m_height(height)
    {}
    virtual glm::vec3 size() {return glm::vec3(m_width,m_height,0);}
    virtual ~RectShape(){}
    float width() const { return m_width; }
    float height() const { return m_height; }
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory& factory) override;
private:
    float m_width;
    float m_height;
};
