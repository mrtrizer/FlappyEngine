#ifndef GWORLDVIEW_H
#define GWORLDVIEW_H

#include <memory>

#include "gviewfactory.h"
#include "gobjcamera.h"

class GWorldModel;
class GViewFactory;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class GWorldView {
public:
    typedef std::shared_ptr<GWorldModel> GWorldModelP;
    typedef std::list<std::shared_ptr<GPresenter>> GPresenterList;

    virtual ~GWorldView();
    void setGWorldModel(GWorldModelP gWorldModel);
    void redrawWorld();
    void resize(int width, int height);
    virtual void init() = 0;
    void updateSize();

protected:
    virtual void redraw(GPresenterList &, GTools::PMatrix &) = 0;

private:
    int width = 1;
    int height = 1;
    GWorldModelP gWorld;

    GWorldModelP getGWorld() { return gWorld; }
    virtual void updateViewPort(int width, int height) = 0;
};

#endif // GWORLDVIEW_H
