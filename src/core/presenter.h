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
    virtual const glm::vec3& size() const {return m_size;}
    virtual void setSize(const glm::vec3& size);
    virtual void setSize(const glm::vec2& size) { setSize(glm::vec3{size.x, size.y, 0}); }
    virtual void update(TimeDelta) {}

protected:
    /// Implement and call appropriate method from factory to get your own instance of GView.
    virtual std::shared_ptr<View> makeGView(const ViewFactory & factory) = 0;
private:
    std::shared_ptr<View> m_view = nullptr;
    glm::vec3 m_size;
};

} // flappy
