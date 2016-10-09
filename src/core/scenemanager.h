#pragma once

#include <core/manager.h>

namespace flappy {

class Scene;

class SceneManager: public Manager<>
{
public:
    void setScene(const std::shared_ptr<Scene>& scene);
    std::shared_ptr<Scene> scene() const { return m_scene; }

private:
    std::shared_ptr<Scene> m_scene;
};

} // flappy
