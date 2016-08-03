#include "view.h"

namespace flappy {

/// Calls update method if need (if externUpdate() was invoked before)
/// and than calls virtual draw() method, reimplemented in concrete childs.
void View::redraw(const mat4 & pMartrix, const mat4 & mvMatrix) {
    auto gPresenterLock = m_gPresenter.lock();
    if (gPresenterLock != nullptr) {
        update(*gPresenterLock);
        m_gPresenter.reset();
    }
    draw(pMartrix, mvMatrix);
}

/// Sets a gPresenter serving as a flag to upgdate view
void View::externUpdate(const shared_ptr<Presenter> & gPresenter) {
    this->m_gPresenter = gPresenter;
}

} // flappy
