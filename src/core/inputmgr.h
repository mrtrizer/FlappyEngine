#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glm/vec3.hpp>
#include <core/tools.h>
#include <core/entitymgr.h>
#include <core/manager.h>

class InputMgr: public Manager<InputMgr>
{
public:
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

namespace Input
{
    inline bool isMouseDown() {return FlappyApp::inst().inputMgr()->isMouseDown();}
    inline bool isMouseUp() {return FlappyApp::inst().inputMgr()->isMouseUp();}
    inline bool isMousePressed() {return FlappyApp::inst().inputMgr()->isMousePressed();}
    inline glm::vec3 getMousePos() {return FlappyApp::inst().inputMgr()->getMousePos();}
}

#endif // INPUTMANAGER_H
