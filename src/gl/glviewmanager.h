#pragma once

#include <vector>

#include <core/viewmanager.h>

namespace flappy {

class GLViewFactory;

/// View implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLViewManager : public ViewManager {
public:
    GLViewManager(const std::shared_ptr<GLViewFactory> & factory):
        m_factory(factory)
    {

    }
    virtual ~GLViewManager();
    void redraw(std::list<Visual> &, glm::mat4 &) override;
    void init() override;
protected:
    void updateViewPort() override;
private:
    std::shared_ptr<GLViewFactory> m_factory;
};

} // flappy
