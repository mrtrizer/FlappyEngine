#include "gpresenter.h"
#include "gviewfactory.h"
#include "gview.h"

/// Returns GView instance and creates new if gView is nullptr.
std::shared_ptr<GView> GPresenter::getGView(const GViewFactory &factory) {
    if (gView == nullptr)
        gView = makeGView(factory);
    return gView;
}

/// Clear a shared pointer to GView instance. Used by GWorldView destructor.
/// shared pointer will reinitilized in next getGView call.
void GPresenter::cleanGView(){
    gView = nullptr;
}

/// Calls GView::externUpdate if gView is set.
/// The idea is not entirely implemented. This method is
/// called only if frameN is changed in GPresenterSprite.
void GPresenter::updateView(){
    if (gView != nullptr)
        gView->externUpdate(shared_from_this_cast<GPresenter>());
}

/// Changes current frame pointer and informs GView about it.
void GPresenterSprite::setFrameN(int frameN) {
    this->frameN = frameN;
    updateView();
}

std::shared_ptr<GView> GPresenterSprite::makeGView(const GViewFactory &factory) {
    return factory.getGViewSprite(*this);
}

std::shared_ptr<GView> GPresenterCircle::makeGView(const GViewFactory &factory) {
    return factory.getGViewCircle(*this);
}

std::shared_ptr<GView> GPresenterRect::makeGView(const GViewFactory &factory) {
    return factory.getGViewRect(*this);
}
