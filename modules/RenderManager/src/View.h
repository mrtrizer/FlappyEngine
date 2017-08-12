#pragma once

#include <memory>
#include <glm/mat4x4.hpp>
#include <Component.h>

#include "Tools.h"

namespace flappy {

/// @brief Interface for all object views.
/// Contains abstract draw() method for implementation in
/// derived classes.
class View : public Component<View> {
public:
    View(TypeIdList dependenceManagerIdList = {}, TypeIdList dependenceComponentList = {});
    virtual ~View() = default;
    void redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix);
protected:
    virtual void draw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) = 0;

private:
    void init() override;
    void deinit() override;
};

} // flappy
