#ifndef GLWORLDVIEW_H
#define GLWORLDVIEW_H

#include <vector>

#include "core/gworldview.h"

class GLViewFactory;

/// View implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLWorldView : public GWorldView {
public:
    GLWorldView(const std::shared_ptr<GLViewFactory> & factory):
        factory(factory)
    {}
    virtual ~GLWorldView();
    void redraw(GPresenterList &, GTools::PMatrix &) override;
    void init() override;
protected:
    void updateViewPort(int width, int height) override;
private:
    std::shared_ptr<GLViewFactory> factory;
};

#endif // GLWORLDVIEW_H
