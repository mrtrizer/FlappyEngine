#include "presenter.h"
#include "viewfactory.h"
#include "view.h"
#include "tools.h"

namespace flappy {

using namespace std;

/// Returns GView instance and creates new if gView is nullptr.
shared_ptr<View> Presenter::getGView(const ViewFactory &factory) {
    if (m_view == nullptr)
        m_view = makeGView(factory);
    return m_view;
}

/// Clear a shared pointer to GView instance. Used by GWorldView destructor.
/// shared pointer will reinitilized in next getGView call.
void Presenter::cleanGView(){
    m_view = nullptr;
}

/// Calls GView::externUpdate if gView is set.
/// The idea is not entirely implemented. This method is
/// called only if frameN is changed in GPresenterSprite.
void Presenter::updateView(){
    if (m_view != nullptr)
        m_view->externUpdate(shared_from_this());
}

} // flappy
