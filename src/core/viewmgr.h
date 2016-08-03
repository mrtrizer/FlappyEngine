#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "viewmgr.h"
#include "viewfactory.h"
#include "camera.h"
#include "transform.h"

namespace flappy {

using namespace std;
using namespace glm;

class ViewFactory;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class ViewMgr: public Manager<ViewMgr> {
public:
    struct Visual {
        shared_ptr<Presenter> presenter;
        mat4 pos;
        float z;
    };

    void update(TimeDelta dt);
    void resize(int width, int height);
    virtual void init() = 0;
    void updateSize();

    class no_camera {};
    
protected:
    virtual void redraw(list<Visual> &, mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;
};

} // flappy
