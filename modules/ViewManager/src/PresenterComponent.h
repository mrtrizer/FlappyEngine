#pragma once

#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <Color.h>
#include <SceneManager.h>
#include <Tools.h>

namespace flappy {

class ViewFactory;
class View;

/// @brief Holds information about object representation need for view objects.
/// @details Contains a shared pointer to GView object, but doesn't calls any it's
/// methods except GView::externUpdate to say about state changing.
class PresenterComponent: public Component, public std::enable_shared_from_this<PresenterComponent> {
public:
    virtual ~PresenterComponent() = default;
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
class CircleShape  : public PresenterComponent {
public:
    unsigned id() override {return ClassId<PresenterComponent,CircleShape>::id(); }
};

/// Represents a rectangle shape.
class RectShape : public PresenterComponent {
    unsigned id() override {return ClassId<PresenterComponent,RectShape>::id(); }
};

} // flappy
