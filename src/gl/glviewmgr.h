#pragma once

#include <vector>

#include <core/viewmgr.h>

namespace flappy {

class GLViewFactory;

/// View implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLViewMgr : public ViewMgr {
public:
    GLViewMgr(const std::shared_ptr<GLViewFactory> & factory):
        factory(factory)
    {

    }
    virtual ~GLViewMgr();
    void redraw(std::list<Visual> &, glm::mat4 &) override;
    void init() override;
protected:
    void updateViewPort() override;
private:
    std::shared_ptr<GLViewFactory> factory;
};

} // flappy
