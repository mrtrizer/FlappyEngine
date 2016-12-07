#pragma once

#include <Manager.h>

namespace flappy {

class Scene;

class SceneManager: public Manager
{
public:
    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> scene() const { return m_scene; }
    void update(float) override;

private:
    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Scene> m_nextScene;
};

} // flappy
