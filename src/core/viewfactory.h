#pragma once

#include <memory>
#include <string>

namespace flappy {

class View;

/// @brief Abstract fabric of GView
/// @details Implementations of this class creates
/// instatances of GView implementations.
/// Constructor methods are called by GPresenter::makeGView()
class ViewFactory
{
public:
    ViewFactory(){}

    virtual ~ViewFactory() {}

    /// Should be specialized for every child of Presenter
    template <typename PresenterT>
    std::shared_ptr<View> get(const PresenterT&) const;
private:
    const int m_circleVectexCnt = 30;
};

} // flappy
