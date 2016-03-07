#ifndef GVIEW_H
#define GVIEW_H

#include <memory>

#include "gtools.h"

class GPresenter;

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class GView : public std::enable_shared_from_this<GView> {
public:
    typedef float * PMatrix;
    typedef float * MVMatrix;
    virtual ~GView(){}
    void redraw(const PMatrix & pMartrix, const MVMatrix & mvMatrix);
    void externUpdate(const std::shared_ptr<GPresenter> & gPresenter);
protected:
    virtual void draw(const PMatrix, const MVMatrix) = 0;
    virtual void update(const GPresenter &) {}
private:
    std::weak_ptr<GPresenter> gPresenter;
};

#endif // GVIEW_H
