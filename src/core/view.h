#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "tools.h"

namespace flappy {

using namespace std;
using namespace glm;

class Presenter;

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class View : public enable_shared_from_this<View> {
public:
    virtual ~View(){}
    void redraw(const mat4 & pMartrix, const mat4 & mvMatrix);
    void externUpdate(const shared_ptr<Presenter> & m_gPresenter);
protected:
    virtual void draw(const mat4 & pMartrix, const mat4 & mvMatrix) = 0;
    virtual void update(const Presenter &) {}
private:
    weak_ptr<Presenter> m_gPresenter;
};

} // flappy
