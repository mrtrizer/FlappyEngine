#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H

#include "presenter.h"

// TODO: Replace r with width and height
/// Represents a circle shape.
class CircleShape  : public Presenter {
public:
    CircleShape(float r):
        m_r(r)
    {}
    virtual ~CircleShape(){}
    float r() const { return m_r; }
    virtual glm::vec3 size() {return glm::vec3(m_r * 2,m_r * 2,0);}
protected:
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) override;
private:
    float m_r;
};

#endif // CIRCLESHAPE_H
