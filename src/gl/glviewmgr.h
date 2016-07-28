#ifndef GLWORLDVIEW_H
#define GLWORLDVIEW_H

#include <vector>

#include <core/viewmgr.h>

class GLViewFactory;

/// View implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLViewMgr : public ViewMgr {
public:
    GLViewMgr(const std::shared_ptr<GLViewFactory> & factory):
        factory(factory)
    {
        init();
        resize(100,100);
    }
    virtual ~GLViewMgr();
    void redraw(GPresenterList &, glm::mat4 &) override;
    void init() override;
protected:
    void updateViewPort() override;
private:
    std::shared_ptr<GLViewFactory> factory;
};

#endif // GLWORLDVIEW_H
