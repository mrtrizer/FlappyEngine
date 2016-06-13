#ifndef GWORLDVIEW_H
#define GWORLDVIEW_H

#include <memory>
#include <entityx/entityx.h>

#include "gviewfactory.h"
#include "gobjcamera.h"
#include "gpos.h"

class GWorldModel;
class GViewFactory;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class GWorldView: public entityx::System<GWorldView> {
public:
    typedef std::shared_ptr<GWorldModel> GWorldModelP;
    struct Visual {
        std::shared_ptr<GPresenter> presenter;
        GPos pos;
    };

    typedef std::list<Visual> GPresenterList;

    virtual ~GWorldView();
    void setGWorldModel(GWorldModelP gWorldModel);
    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override;
    void resize(int width, int height);
    virtual void init() = 0;
    void updateSize();

protected:
    virtual void redraw(GPresenterList &, GTools::PMatrix &) = 0;

private:
    int width = 1;
    int height = 1;
    bool updateSizeFlag = true;
    GWorldModelP gWorld;

    GWorldModelP getGWorld() { return gWorld; }
    virtual void updateViewPort(int width, int height) = 0;
};

#endif // GWORLDVIEW_H
