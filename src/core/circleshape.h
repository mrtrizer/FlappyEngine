#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H

#include "presenter.h"

/// Represents a circle shape.
class CircleShape  : public Presenter {
public:
    CircleShape(float r):
        r(r)
    {}
    virtual ~CircleShape(){}
    inline float getR_() const { return r; }
    virtual glm::vec3 getSize() {return glm::vec3(r * 2,r * 2,0);}
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    float r;
};

#endif // CIRCLESHAPE_H
