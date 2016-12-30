#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "Tools.h"

namespace flappy {

class PresenterComponent;

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class View : public std::enable_shared_from_this<View> {
public:
    virtual ~View() = default;
    void redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix);
    void externUpdate(const std::shared_ptr<PresenterComponent> & m_gPresenter);
protected:
    virtual void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) = 0;
    virtual void update(const PresenterComponent &) {}
private:
    std::weak_ptr<PresenterComponent> m_gPresenter;
};

} // flappy
