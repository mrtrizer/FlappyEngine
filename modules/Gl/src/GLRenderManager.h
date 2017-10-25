#pragma once

#include <vector>

#include <RenderManager.h>

namespace flappy {

class GLRenderFactory;

/// Render implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class GLRenderManager : public RenderManager {
public:
    GLRenderManager();
    void redraw(std::list<Visual> &, glm::mat4 &) final;
protected:
    void updateViewPort() final;
};

} // flappy