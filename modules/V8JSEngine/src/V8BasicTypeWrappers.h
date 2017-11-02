#pragma once

#include <Manager.h>

#include <v8.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace flappy {

using _Bool = bool;

template <typename T>
v8::Local<v8::Value> toV8(const T&);

template <typename T>
T toCpp(v8::Local<v8::Value>);

} // flappy
