#pragma once

#include <chrono>
#include <memory>

namespace flappy {

using namespace std;

class ViewMgr;
class EntityMgr;
class InputMgr;
class SceneMgr;
class ScreenMgr;
class GameMgr;
class ResourceMgr;

class FlappyApp {
public:
    FlappyApp() {}
    FlappyApp(const FlappyApp&) = delete;
    void operator=(const FlappyApp&) = delete;

    static FlappyApp& inst() {
        static FlappyApp s_flappy;
        return s_flappy;
    }

    void update();
    void configure();

    void setWorldView(shared_ptr<ViewMgr> worldView) { m_viewMgr = worldView; }
    void setGameMgr(shared_ptr<GameMgr> gameMgr) {m_gameMgr = gameMgr;}
    void setResourceMgr(shared_ptr<ResourceMgr> resourceMgr) {m_resourceMgr = resourceMgr;}

    shared_ptr<ViewMgr> viewMgr() const {return m_viewMgr;}
    shared_ptr<EntityMgr> entityMgr() const {return m_entityMgr;}
    shared_ptr<InputMgr> inputMgr() const {return m_inputMgr;}
    shared_ptr<SceneMgr> sceneMgr() const {return m_sceneMgr;}
    shared_ptr<ScreenMgr> screenMgr() const {return m_screenMgr;}
    shared_ptr<ResourceMgr> resourceMgr() const {return m_resourceMgr;}

private:
    bool m_configured = false;
    chrono::steady_clock::time_point m_lastTime;
    shared_ptr<ViewMgr> m_viewMgr;
    shared_ptr<EntityMgr> m_entityMgr;
    shared_ptr<InputMgr> m_inputMgr;
    shared_ptr<SceneMgr> m_sceneMgr;
    shared_ptr<ScreenMgr> m_screenMgr;
    shared_ptr<GameMgr> m_gameMgr;
    shared_ptr<ResourceMgr> m_resourceMgr;
};

} // flappy
