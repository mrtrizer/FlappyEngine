#ifndef GCONTEXT_H
#define GCONTEXT_H

/// @brief Context contains current input controllers
/// state. (Only mouse yet)
/// @details It is passed to GObj::recalc method. So, all objects
/// can use it.
struct GContext {
    enum MouseEvent {EMPTY, CLICK, PRESS, RELEASE};
    GContext(int x = 0, int y = 0, MouseEvent mouseEvent = EMPTY):
        x(x), y(y),
        mouseEvent(mouseEvent)
    {}
    inline int getX() const {return x;}
    inline int getY() const {return y;}
    inline MouseEvent getMouseEvent() const {return mouseEvent;}
private:
    int x;
    int y;
    MouseEvent mouseEvent;
};

#endif // GCONTEXT_H
