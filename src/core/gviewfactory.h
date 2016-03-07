#ifndef GVIEWFACTORY_H
#define GVIEWFACTORY_H

#include <memory>
#include <string>

#include "gpresenter.h"

class GView;
class GLTexture;

/// @brief Abstract fabric of GView
/// @details Implementations of this class creates
/// instatances of GView implementations.
/// Constructor methods are called by GPresenter::makeGView()
class GViewFactory
{
public:
    typedef std::shared_ptr<GView> GViewP;

    virtual ~GViewFactory() {}
    virtual GViewP getGViewSprite(const GPresenter &) const = 0;
    virtual GViewP getGViewCircle(const GPresenter & ) const = 0;
    virtual GViewP getGViewRect(const GPresenter & ) const = 0;
};

#endif // GVIEWFACTORY_H
