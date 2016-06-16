#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <memory>
#include <glm/vec2.hpp>

class ScreenManager
{
public:
    ScreenManager();
    static std::shared_ptr<ScreenManager> getInst() {return inst == nullptr? inst = std::make_shared<ScreenManager>(): inst;}
    int width = 1;
    int height = 1;
    float coefff = 1.0f;

    inline glm::vec2 getScreenSize() const {
        return glm::vec2(width, height);
    }

private:
    static std::shared_ptr<ScreenManager> inst;
};

#endif // SCREENMANAGER_H
