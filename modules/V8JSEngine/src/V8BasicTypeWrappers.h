#pragma once

#include <Manager.h>

#include <v8.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace flappy {

v8::Local<v8::Value> std__string_to_v8(std::string);
v8::Local<v8::Value> int_to_v8(int);
v8::Local<v8::Value> float_to_v8(float);
v8::Local<v8::Value> _Bool_to_v8(bool);
v8::Local<v8::Value> glm__vec3_to_v8(glm::vec3);
v8::Local<v8::Value> glm__vec2_to_v8(glm::vec2);
v8::Local<v8::Value> glm__quat_to_v8(glm::quat);

std::string v8_to_std__string(v8::Local<v8::Value>);
int v8_to_int(v8::Local<v8::Value>);
float v8_to_float(v8::Local<v8::Value>);
bool v8_to__Bool(v8::Local<v8::Value>);
glm::vec3 v8_to_glm__vec3(v8::Local<v8::Value>);
glm::vec2 v8_to_glm__vec2(v8::Local<v8::Value>);
glm::quat glm__quat_to_v8(v8::Local<v8::Value>);

} // flappy
