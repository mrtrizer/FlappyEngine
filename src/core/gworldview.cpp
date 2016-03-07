#include "gworldview.h"
#include "gworldmodel.h"

GWorldView::~GWorldView(){
    //clean presenters
    auto objects = getGWorld()->getRoot()->findChilds();
    for (std::shared_ptr<GObj> gObj: objects) {
        //If it's a visible object
        auto presenter = std::dynamic_pointer_cast<GPresenter>(gObj);
        if (presenter == nullptr)
            continue;
        presenter->cleanGView();
    }
}

void GWorldView::setGWorldModel(GWorldModelP gWorldModel) {
    this->gWorld = gWorldModel;
    updateSize();
}

void GWorldView::redrawWorld() {
    if (getGWorld() == nullptr)
        return;

    //Calc projection matrix, using GObjCamera
    auto pMatrix = getGWorld()->getActiveCamera()->getPMatrix();

    std::list<std::shared_ptr<GPresenter>> presenters;

    getGWorld()->getRoot()->findChilds([&presenters](const GObj::GObjP & i) {
        auto presenter = std::dynamic_pointer_cast<GPresenter>(i);
        if (presenter != nullptr)
            presenters.push_back(presenter);
        return true;
    },true);

    redraw(presenters, pMatrix);
}

void GWorldView::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw std::runtime_error("Invalid screen size. Has to be > 0.");
    this->width = width;
    this->height = height;
    updateSize();
}

void GWorldView::updateSize() {
    if (gWorld != nullptr)
        gWorld->getActiveCamera()->resize(width, height);
    updateViewPort(width, height);
}
