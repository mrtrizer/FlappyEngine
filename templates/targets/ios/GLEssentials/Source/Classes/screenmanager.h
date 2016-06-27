#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <memory>
#include <glm/vec2.hpp>
#include <core/manager.h>

class ScreenManager: public Manager<ScreenManager>
{
public:
    inline glm::vec2 getScreenSize() const {
        return glm::vec2(width, height);
    }

    int width = 1;
    int height = 1;
    float coefff = 1.0f;
};

#endif // SCREENMANAGER_H
