#pragma once

#include <chrono>
#include <memory>
#include <vector>



namespace flappy {

using namespace std;

class ViewMgr;
class EntityMgr;
class GameMgr;
class ResourceMgr;
class IManager;

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
    void init();

    template <typename Mgr> inline
    Mgr* MGR() {
        return reinterpret_cast<Mgr*>(m_mgrList[Mgr::counter.id()].get());
    }

    template <typename Mgr, typename...T> inline
    void createMgr(T&&...args) {
        addMgr<Mgr>(make_shared<Mgr>(forward<T>(args)...));
    }

    template <typename IMgr, typename Mgr, typename ... T> inline
    void overrideMgr(T&&...args) {
        setMgrAtPos(IMgr::counter.id(), make_shared<Mgr>(forward<T>(args)...));
    }

    template <typename Mgr> inline
    void addMgr(shared_ptr<Mgr> mgr) {
        setMgrAtPos(Mgr::counter.id(), mgr);
    }

    void setMgrAtPos(unsigned int pos, shared_ptr<IManager> mgr);

private:
    chrono::steady_clock::time_point m_lastTime;
    vector<shared_ptr<IManager>> m_mgrList;
};

} // flappy
