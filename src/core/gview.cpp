#include "gview.h"

/// Calls update method if need (if externUpdate() was invoked before)
/// and than calls virtual draw() method, reimplemented in concrete childs.
void GView::redraw(const glm::mat4 & pMartrix, const glm::mat4 & mvMatrix) {
    auto gPresenterLock = gPresenter.lock();
    if (gPresenterLock != nullptr) {
        update(*gPresenterLock);
        gPresenter.reset();
    }
    draw(pMartrix, mvMatrix);
}

/// Sets a gPresenter serving as a flag to upgdate view
void GView::externUpdate(const std::shared_ptr<GPresenter> & gPresenter) {
    this->gPresenter = gPresenter;
}
