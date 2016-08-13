#pragma once

#include <chrono>
#include <memory>
#include <vector>

namespace flappy {

class ViewMgr;
class EntityMgr;
class GameMgr;
class ResourceMgr;
class IManager;

class FlappyApp : public std::enable_shared_from_this<FlappyApp> {
public:
    FlappyApp() {}
    FlappyApp(const FlappyApp&) = delete;
    void operator=(const FlappyApp&) = delete;

    void update();
    void init();

    template <typename Mgr> inline
    Mgr* MGR() {
        return reinterpret_cast<Mgr*>(m_mgrList[Mgr::counter.id()].get());
    }

    template <typename Mgr, typename...T> inline
    void createMgr(T&&...args) {
        addMgr<Mgr>(std::make_shared<Mgr>(std::forward<T>(args)...));
    }

    template <typename IMgr, typename Mgr, typename ... T> inline
    void overrideMgr(T&&...args) {
        setMgrAtPos(IMgr::counter.id(), std::make_shared<Mgr>(std::forward<T>(args)...));
    }

    template <typename Mgr> inline
    void addMgr(std::shared_ptr<Mgr> mgr) {
        setMgrAtPos(Mgr::counter.id(), mgr);
    }

    void setMgrAtPos(unsigned int pos, std::shared_ptr<IManager> mgr);

private:
    std::chrono::steady_clock::time_point m_lastTime;
    std::vector<std::shared_ptr<IManager>> m_mgrList;
};

} // flappy
