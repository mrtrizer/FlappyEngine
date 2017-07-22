#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include <Manager.h>
#include <CameraComponent.h>
#include <TransformComponent.h>

namespace flappy {

class View;
class SceneManager;
class ScreenManager;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class ViewManager: public Manager<ViewManager> {
private:

    class IViewFactory {
    public:
        virtual ~IViewFactory() = default;
        virtual std::shared_ptr<View> build() = 0;
    };

    template <typename ViewT>
    class ViewFactory: public IViewFactory {
    public:
        std::shared_ptr<View> build() { return std::make_shared<ViewT>(); }
    };

public:
    struct OnWindowResize: public IEvent {
        float width;
        float height;
    };

    ViewManager(TypeIdList);

    void update(DeltaTime dt);

    void registerRenderElement(const SafePtr<View> renderElement);

protected:
    struct Visual {
        SafePtr<View> view;
        glm::mat4 pos;
        float z;
    };

    virtual void redraw(std::list<Visual> &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;

    std::list<Visual> m_visuals;
};

} // flappy
