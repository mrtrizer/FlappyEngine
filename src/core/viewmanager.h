#ifndef GWORLDVIEW_H
#define GWORLDVIEW_H

#include <memory>
#include <glm/mat4x4.hpp>

#include "viewfactory.h"
#include "camera.h"
#include "transform.h"

class ViewFactory;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class ViewManager {
public:
    struct Visual {
        std::shared_ptr<Presenter> presenter;
        glm::mat4 pos;
        float z;
    };

    
    ViewManager() {
        instance = this;
    }
    
    typedef std::list<Visual> GPresenterList;

    virtual ~ViewManager();
    void update(TimeDelta dt);
    void resize(int width, int height);
    virtual void init() = 0;
    void updateSize();

    class no_camera {};

    static ViewManager* instance;
    
protected:
    virtual void redraw(GPresenterList &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;
    
};

#endif // GWORLDVIEW_H
