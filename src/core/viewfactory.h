#ifndef GVIEWFACTORY_H
#define GVIEWFACTORY_H

#include <memory>
#include <string>

#include "presenter.h"

class View;

/// @brief Abstract fabric of GView
/// @details Implementations of this class creates
/// instatances of GView implementations.
/// Constructor methods are called by GPresenter::makeGView()
class ViewFactory
{
public:
    typedef std::shared_ptr<View> GViewP;

    virtual ~ViewFactory() {}
    virtual GViewP getGViewSprite(const Presenter &) const = 0;
    virtual GViewP getGViewCircle(const Presenter & ) const = 0;
    virtual GViewP getGViewRect(const Presenter & ) const = 0;
};

#endif // GVIEWFACTORY_H
