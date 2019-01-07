#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include <IEvent.h>
#include <CameraComponent.h>
#include <TransformComponent.h>
#include <Updatable.hpp>

#include "Render.h"

namespace flappy {

class SceneManager;
class ScreenManager;

/// @brief Abstract base for Render implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class REFLECT RenderManager : public Updatable<RenderManager> {
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

    RenderManager(Handle<Hierarchy> hierarchy);

    void update(DeltaTime dt);

    void registerRenderElement(const Handle<Render> renderElement);

    void unregisterRenderElement(const Handle<Render> renderElement);

protected:
    struct Visual {
        Handle<Render> view;
        glm::mat4 pos;
        float z;
    };

    virtual void redraw(std::list<Visual> &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;

    Handle<SceneManager> m_sceneManager;
    std::list<Visual> m_visuals;
};

} // flappy
