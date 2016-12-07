#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "ViewManager.h"
#include <CameraComponent.h>
#include <TransformComponent.h>

namespace flappy {

class PresenterComponent;
class View;
class EntityManager;
class ScreenManager;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class ViewManager: public Manager {
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
    ViewManager(): m_bindings(ClassCounter<PresenterComponent>::count()) {}

    void update(TimeDelta dt);
    void resize(int width, int height);

    /// Assing custom View for PresenterComponent
    template <typename PresenterT, typename ViewT>
    void bind() {
        m_bindings[ClassId<PresenterComponent,PresenterT>::id()] = std::make_shared<ViewFactory<ViewT>>();
    }
    
    /// Create View for particular presenter.
    /// @details Before using of this method, you need assign
    /// custom View implementation for PresenterComponent,
    /// using bind() method.
    void addPresenter(const std::shared_ptr<PresenterComponent>& presenter);

protected:
    struct Visual {
        std::shared_ptr<PresenterComponent> presenter;
        std::shared_ptr<View> view;
        glm::mat4 pos;
        float z;
    };

    virtual void redraw(std::list<Visual> &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;

    std::vector<std::shared_ptr<IViewFactory>> m_bindings;
    std::list<Visual> m_visuals;
};

} // flappy
