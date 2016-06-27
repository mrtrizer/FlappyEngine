#ifndef GWORLDVIEW_H
#define GWORLDVIEW_H

#include <memory>
#include <entityx/entityx.h>
#include <glm/mat4x4.hpp>

#include "gviewfactory.h"
#include "ccamera.h"
#include "ctransform.h"

class GViewFactory;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class GWorldView: public entityx::System<GWorldView> {
public:
    struct Visual {
        std::shared_ptr<GPresenter> presenter;
        glm::mat4 pos;
        float z;
    };

    typedef std::list<Visual> GPresenterList;

    GWorldView() {
        instance = this;
    }
    virtual ~GWorldView();
    void update(entityx::EntityManager &es, entityx::EventManager &, entityx::TimeDelta dt) override;
    void resize(int width, int height);
    virtual void init() = 0;
    void updateSize();
    class no_camera {};
    
    static GWorldView* getInst(){return instance;}

protected:
    virtual void redraw(GPresenterList &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;
    static GWorldView* instance;
};

#endif // GWORLDVIEW_H
