#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <v8.h>

#include <Component.h>

namespace flappy
{

class JSComponent: public Component<JSComponent>
{
public:
    JSComponent(std::string name, std::string script);

private:
    v8::UniquePersistent<v8::Object> m_jsObject;
};

} // flappy
