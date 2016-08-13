#pragma once

#include <memory>
#include <string>

#include "texture.h"
#include "presenter.h"

namespace flappy {

class View;

/// @brief Abstract fabric of GView
/// @details Implementations of this class creates
/// instatances of GView implementations.
/// Constructor methods are called by GPresenter::makeGView()
class ViewFactory
{
public:
    ViewFactory(std::weak_ptr<FlappyApp> flappyApp):
        m_flappyApp(flappyApp) {

    }

    virtual ~ViewFactory() {}

    /// Should be specialized for every child of Presenter
    template <typename PresenterT>
    std::shared_ptr<View> get(const PresenterT&) const;
protected:
    std::weak_ptr<FlappyApp> flappyApp() const { return m_flappyApp; }

private:
    const int m_circleVectexCnt = 30;

    std::weak_ptr<FlappyApp> m_flappyApp;
};

} // flappy
