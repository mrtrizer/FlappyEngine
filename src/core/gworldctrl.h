#ifndef GWORLDCTRL_H
#define GWORLDCTRL_H

#include <memory>
#include <queue>

#include "gcontext.h"

class GWorldView;
class GWorldModel;

class GWorldCtrl {
public:
    typedef std::shared_ptr<GWorldView> GWorldViewP;
    typedef std::shared_ptr<GWorldModel> GWorldModelP;

    virtual void init(){}
    void mouseClick(int x, int y);
    void mouseMove(int x, int y);
    void step();
    void setView(const GWorldViewP &gWorldView);
    void setWorld(const GWorldModelP & gWorld);
    GWorldModelP getCurWorld() { return curWorld; }

protected:
    virtual void procState(){}

private:
    GWorldModelP curWorld;
    GWorldViewP gWorldView;
    std::queue<GContext> events;
};

#endif // GWORLDCTRL_H
