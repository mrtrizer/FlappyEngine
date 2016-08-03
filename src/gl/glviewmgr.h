#pragma once

#include <vector>

#include <core/viewmgr.h>

namespace flappy {

using namespace std;
using namespace glm;

class GLViewFactory;

/// View implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLViewMgr : public ViewMgr {
public:
    GLViewMgr(const shared_ptr<GLViewFactory> & factory):
        factory(factory)
    {
        init();
        resize(100,100);
    }
    virtual ~GLViewMgr();
    void redraw(list<Visual> &, mat4 &) override;
    void init() override;
protected:
    void updateViewPort() override;
private:
    shared_ptr<GLViewFactory> factory;
};

} // flappy
