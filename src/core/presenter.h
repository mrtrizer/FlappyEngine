#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "color.h"
#include "entitymanager.h"
#include "tools.h"

namespace flappy {

class ViewFactory;
class View;

/// @brief Holds information about object representation need for view objects.
/// @details Contains a shared pointer to GView object, but doesn't calls any it's
/// methods except GView::externUpdate to say about state changing.
class Presenter: public Component, public std::enable_shared_from_this<Presenter> {
public:
    Presenter() = default;
    virtual ~Presenter() = default;
    void updateView();
    virtual void update(TimeDelta) {}
    void setColor(const Color& color);
    Color color() const { return m_color; }
    virtual unsigned id() = 0;
    void init() override;
    void setView(std::weak_ptr<View> view) { m_view = view; }

private:
    std::weak_ptr<View> m_view;
    Color m_color;
};

} // flappy
