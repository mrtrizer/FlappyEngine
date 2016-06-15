#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <memory>

class ScreenManager
{
public:
    ScreenManager();
    static std::shared_ptr<ScreenManager> getInst() {return inst == nullptr? inst = std::make_shared<ScreenManager>(): inst;}
    int width = 1;
    int height = 1;
private:
    static std::shared_ptr<ScreenManager> inst;
};

#endif // SCREENMANAGER_H
