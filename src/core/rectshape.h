#ifndef RECTSHAPE_H
#define RECTSHAPE_H

#include "presenter.h"

/// Represents a rectangle shape.
class RectShape : public Presenter {
public:
    RectShape(float width, float height):
        width(width),
        height(height)
    {}
    virtual glm::vec3 getSize() {return glm::vec3(width,height,0);}
    virtual ~RectShape(){}
    inline float getWidth() const { return width; }
    inline float getHeight() const { return height; }
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    float width;
    float height;
};
#endif // RECTSHAPE_H
