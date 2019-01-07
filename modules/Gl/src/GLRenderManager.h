#pragma once

#include <vector>

#include <RenderManager.h>

namespace flappy {

class GLRenderFactory;
class IGLManager;

/// Render implementation for work with OpenGL.
/// Definitly supports GLES 2.0 (Android) and OpenGL 4.5.
class REFLECT GLRenderManager : public RenderManager {
public:
    GLRenderManager(Handle<Hierarchy> hierarchy);
    virtual ~GLRenderManager() = default;
protected:
    void updateViewPort() final;
    void redraw(std::list<Visual> &, glm::mat4 &) final;
    Handle<IGLManager> m_glManager;
    Handle<ScreenManager> m_screenManager;
};

} // flappy
