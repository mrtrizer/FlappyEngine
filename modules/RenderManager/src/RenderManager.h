#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include <Manager.h>
#include <CameraComponent.h>
#include <TransformComponent.h>

namespace flappy {

class Render;
class SceneManager;
class ScreenManager;

/// @brief Abstract base for Render implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class RenderManager: public Manager<RenderManager> {
private:

    class IViewFactory {
    public:
        virtual ~IViewFactory() = default;
        virtual std::shared_ptr<Render> build() = 0;
    };

    template <typename ViewT>
    class ViewFactory: public IViewFactory {
    public:
        std::shared_ptr<Render> build() { return std::make_shared<ViewT>(); }
    };

public:
    struct OnWindowResize: public IEvent {
        float width;
        float height;
    };

    RenderManager();

    void update(DeltaTime dt);

    void registerRenderElement(const SafePtr<Render> renderElement);

    void unregisterRenderElement(const SafePtr<Render> renderElement);

protected:
    struct Visual {
        SafePtr<Render> view;
        glm::mat4 pos;
        float z;
    };

    virtual void redraw(std::list<Visual> &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;

    std::list<Visual> m_visuals;
};

} // flappy
