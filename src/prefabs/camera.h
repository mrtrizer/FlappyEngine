#ifndef CAMERA_H
#define CAMERA_H

#include <core/prefab.h>
#include <core/ccamera.h>
#include <core/ctransform.h>

class Camera: public Prefab
{
public:
    void init(entityx::EntityManager & entities) override {
        auto camera = entities.create();
        camera.assign<CCamera>(100, 1, 500);
        camera.assign<CTransform>(0,0,-1)->angle = 1;
    }
};

#endif // CAMERA_H
