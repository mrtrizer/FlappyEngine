#pragma once

#include <memory>
#include <glm/mat4x4.hpp>

#include "viewmanager.h"
#include "camera.h"
#include "transform.h"

namespace flappy {

class Presenter;
class View;

/// @brief Abstract base for View implementations in MVC terms.
/// @details Holds a pointer to GWorldModel.
class ViewManager: public Manager {
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
    struct Visual {
        std::shared_ptr<Presenter> presenter;
        std::shared_ptr<View> view;
        glm::mat4 pos;
        float z;
    };

    void update(TimeDelta dt);
    void resize(int width, int height);
    void updateSize();

    template <typename PresenterT, typename ViewT>
    void bind() {
        if (m_bindings.size() <= ClassId<Presenter, PresenterT>::id())
            m_bindings.resize(ClassId<Presenter,PresenterT>::id() + 1);
        m_bindings[ClassId<Presenter,PresenterT>::id()] = std::make_shared<ViewFactory<ViewT>>();
    }

    class no_camera {};
    
    void addPresenter(const std::shared_ptr<Presenter>& presenter);

protected:
    virtual void redraw(std::list<Visual> &, glm::mat4 &) = 0;

private:
    virtual void updateViewPort() = 0;

    std::vector<std::shared_ptr<IViewFactory>> m_bindings;
    std::list<Visual> m_visuals;
};

} // flappy
