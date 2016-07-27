#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glm/vec3.hpp>
#include <core/tools.h>
#include <core/entitymanager.h>
#include <core/manager.h>

class InputMgr: public Manager<InputMgr>
{
public:
    InputMgr();

    bool isMouseDown() const;
    bool isMouseUp() const;
    bool isMousePressed() const;
    glm::vec3 getMousePos() const;

    void update(TimeDelta);
    void setMouseDown();
    void setMouseUp();
    void mouseMove(const glm::vec3& m_mousePos);
private:

    glm::vec3 m_mousePos;
    int m_mouseDown = 0;
    int m_mouseUp = 0;
    bool m_mousePressed = false;
};

namespace Input {
    inline bool isMouseDown() {return InputMgr::getInst()->isMouseDown();}
    inline bool isMouseUp() {return InputMgr::getInst()->isMouseUp();}
    inline bool isMousePressed() {return InputMgr::getInst()->isMousePressed();}
    inline glm::vec3 getMousePos() {return InputMgr::getInst()->getMousePos();}
}

#endif // INPUTMANAGER_H
