#pragma once

#include <string>
#include <memory>
#include <core/entitymgr.h>
#include <glm/glm.hpp>

#include "tools.h"

namespace flappy {

class ViewFactory;
class View;

/// @brief Holds information about object representation need for view objects.
/// @details Contains a shared pointer to GView object, but doesn't calls any it's
/// methods except GView::externUpdate to say about state changing.
class Presenter: public Component, public std::enable_shared_from_this<Presenter> {
public:
    Presenter()
    {}
    virtual ~Presenter(){}
    std::shared_ptr<View> getGView(const ViewFactory & factory);
    void cleanGView();
    void updateView();
    virtual glm::vec3 size() {return glm::vec3();}
    virtual void update(TimeDelta) {}

protected:
    /// Implement and call appropriate method from factory to get your own instance of GView.
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) = 0;
private:
    std::shared_ptr<View> m_view = nullptr;
};

} // flappy
