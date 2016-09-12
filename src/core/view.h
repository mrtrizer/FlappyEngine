#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "tools.h"

namespace flappy {

class Presenter;

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class View : public std::enable_shared_from_this<View> {
public:
    virtual ~View() = default;
    void redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix);
    void externUpdate(const std::shared_ptr<Presenter> & m_gPresenter);
protected:
    virtual void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) = 0;
    virtual void update(const Presenter &) {}
private:
    std::weak_ptr<Presenter> m_gPresenter;
};

} // flappy
