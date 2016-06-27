#ifndef GVIEW_H
#define GVIEW_H

#include <memory>
#include <glm/mat4x4.hpp>

#include "gtools.h"

class GPresenter;

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class GView : public std::enable_shared_from_this<GView> {
public:
    virtual ~GView(){}
    void redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix);
    void externUpdate(const std::shared_ptr<GPresenter> & gPresenter);
protected:
    virtual void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) = 0;
    virtual void update(const GPresenter &) {}
private:
    std::weak_ptr<GPresenter> gPresenter;
};

#endif // GVIEW_H
