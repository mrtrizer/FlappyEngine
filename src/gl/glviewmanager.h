#ifndef GLWORLDVIEW_H
#define GLWORLDVIEW_H

#include <vector>

#include <core/viewmanager.h>

class GLViewFactory;

/// View implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLWorldView : public ViewManager {
public:
    GLWorldView(const std::shared_ptr<GLViewFactory> & factory):
        factory(factory)
    {
        init();
        resize(100,100);
    }
    virtual ~GLWorldView();
    void redraw(GPresenterList &, glm::mat4 &) override;
    void init() override;
protected:
    void updateViewPort() override;
private:
    std::shared_ptr<GLViewFactory> factory;
};

#endif // GLWORLDVIEW_H
