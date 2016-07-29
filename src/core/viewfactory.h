#pragma once

#include <memory>
#include <string>

#include "texture.h"
#include "presenter.h"

class View;

/// @brief Abstract fabric of GView
/// @details Implementations of this class creates
/// instatances of GView implementations.
/// Constructor methods are called by GPresenter::makeGView()
class ViewFactory
{
public:
    virtual ~ViewFactory() {}

    /// Should be specialized for every child of Presenter
    template <typename PresenterT>
    std::shared_ptr<View> get(const PresenterT&) const;
private:
    using TextureMap = std::map<std::string,std::shared_ptr<Texture>>;

    const int m_circleVectexCnt = 30;
    mutable TextureMap m_textureMap;
};
