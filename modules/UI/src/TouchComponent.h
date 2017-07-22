#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Component.h>

namespace flappy
{

class TouchComponent: public Component<TouchComponent>
{
public:
    TouchComponent();

private:
    void init() final;
    void deinit() final;
};

} // flappy
