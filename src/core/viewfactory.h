#pragma once

#include <memory>
#include <string>

#include "texture.h"
#include "presenter.h"

namespace flappy {

using namespace std;

class View;

/// @brief Abstract fabric of GView
/// @details Implementations of this class creates
/// instatances of GView implementations.
/// Constructor methods are called by GPresenter::makeGView()
class ViewFactory
{
public:
    ViewFactory(weak_ptr<FlappyApp> flappyApp):
        m_flappyApp(flappyApp) {

    }

    virtual ~ViewFactory() {}

    /// Should be specialized for every child of Presenter
    template <typename PresenterT>
    shared_ptr<View> get(const PresenterT&) const;
protected:
    weak_ptr<FlappyApp> flappyApp() const { return m_flappyApp; }

private:
    using TextureMap = map<string,shared_ptr<Texture>>;

    const int m_circleVectexCnt = 30;
    mutable TextureMap m_textureMap;

    weak_ptr<FlappyApp> m_flappyApp;
};

} // flappy
