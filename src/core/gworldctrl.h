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
    inline int getScore() const {return score;}
    inline int getBestScore() const {return bestScore;}

protected:
    virtual void procState() = 0;

private:
    GWorldModelP curWorld;
    GWorldViewP gWorldView;
    std::queue<GContext> events;
    int score = 0;
    int bestScore = 0;
};

#endif // GWORLDCTRL_H
