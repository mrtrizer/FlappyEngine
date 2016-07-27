#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <memory>
#include <glm/vec2.hpp>
#include <core/manager.h>

class ScreenMgr: public Manager<ScreenMgr>
{
public:
    glm::vec2 screenSize() const;
    void resize(int width, int height);

    int width() const {return m_width;}
    int height() const {return m_height;}

private:
    int m_width = 1;
    int m_height = 1;
    float m_coefff = 1.0f;
};

namespace Screen
{
    inline glm::vec2 screenSize() {return ScreenMgr::getInst()->screenSize();}
}

#endif // SCREENMANAGER_H
