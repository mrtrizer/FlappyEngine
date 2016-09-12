#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "viewmanager.h"
#include "viewfactory.h"
#include "camera.h"
#include "transform.h"

namespace flappy {

class Presenter;
class ViewFactory;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class ViewManager: public Manager {
public:
    struct Visual {
        std::shared_ptr<Presenter> presenter;
        glm::mat4 pos;
        float z;
    };

    void update(TimeDelta dt);
    void resize(int width, int height);
    void updateSize();

    class no_camera {};
    
protected:
    virtual void redraw(std::list<Visual> &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;
};

} // flappy
