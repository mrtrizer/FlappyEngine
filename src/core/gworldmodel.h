#ifndef GWORLDMODEL_H
#define GWORLDMODEL_H

#include <list>
#include <memory>
#include <chrono>

#include "gobj.h"
#include "gobjcamera.h"

/// @brief Game World
/// @details Contains and manages game objects.
/// Model in MVC terms.
class GWorldModel {
public:
    GWorldModel();
    virtual ~GWorldModel(){}
    inline std::shared_ptr<GObj> getRoot() {return gObj;}
    void run(const GContext &);
    void initWorld();
    void setActiveCamera(const std::shared_ptr<GObjCamera> & camera) {this->activeCamera = camera;}
    std::shared_ptr<GObjCamera> getActiveCamera() const {return activeCamera;}

protected:
    virtual void init(){}
    virtual void recalc(GObj::DeltaT, const GContext &){}

private:
    std::shared_ptr<GObj> gObj;
    std::shared_ptr<GObjCamera> activeCamera;
    std::chrono::steady_clock::time_point lastTime;
};

#endif // GWORLDMODEL_H
