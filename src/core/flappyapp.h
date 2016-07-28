#ifndef FLAPPYAPP_H
#define FLAPPYAPP_H

#include <chrono>

class ViewMgr;
class EntityMgr;
class InputMgr;
class SceneMgr;
class ScreenMgr;
class GameMgr;

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

    void setWorldView(std::shared_ptr<ViewMgr> worldView) { m_viewMgr = worldView; }
    void setGameMgr(std::shared_ptr<GameMgr> gameMgr) {m_gameMgr = gameMgr;}

    std::shared_ptr<ViewMgr> viewMgr() const {return m_viewMgr;}
    std::shared_ptr<EntityMgr> entityMgr() const {return m_entityMgr;}
    std::shared_ptr<InputMgr> inputMgr() const {return m_inputMgr;}
    std::shared_ptr<SceneMgr> sceneMgr() const {return m_sceneMgr;}
    std::shared_ptr<ScreenMgr> screenMgr() const {return m_screenMgr;}

private:
    bool m_configured = false;
    std::chrono::steady_clock::time_point m_lastTime;
    std::shared_ptr<ViewMgr> m_viewMgr;
    std::shared_ptr<EntityMgr> m_entityMgr;
    std::shared_ptr<InputMgr> m_inputMgr;
    std::shared_ptr<SceneMgr> m_sceneMgr;
    std::shared_ptr<ScreenMgr> m_screenMgr;
    std::shared_ptr<GameMgr> m_gameMgr;
};

#endif // FLAPPYAPP_H
