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
    virtual ~Presenter() = default;
    void updateView();
    void update(TimeDelta) override {}
    void setColor(const Color& color);
    Color color() const { return m_color; }
    virtual unsigned id() = 0;
    void init() override;
    void setView(std::weak_ptr<View> view) { m_view = view; }
    std::weak_ptr<View> view() { return m_view; }

private:
    std::weak_ptr<View> m_view;
    Color m_color;
};

/// Represents a circle shape.
class CircleShape  : public Presenter {
public:
    unsigned id() override {return ClassId<Presenter,CircleShape>::id(); }
};

/// Represents a rectangle shape.
class RectShape : public Presenter {
    unsigned id() override {return ClassId<Presenter,RectShape>::id(); }
};

} // flappy
