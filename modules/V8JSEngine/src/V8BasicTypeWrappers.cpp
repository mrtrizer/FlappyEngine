#include "V8BasicTypeWrappers.h"

#include "V8JSManager.h"

namespace flappy {

template <>
bool isMatchCpp<std::string>(v8::Local<v8::Value> value) {
    return value->IsString();
}

template <>
bool isMatchCpp<bool>(v8::Local<v8::Value> value) {
    return value->IsBoolean();
}

template <>
bool isMatchCpp<int>(v8::Local<v8::Value> value) {
    return value->IsNumber();
}

template <>
bool isMatchCpp<float>(v8::Local<v8::Value> value) {
    return value->IsNumber();
}

template <>
bool isMatchCpp<glm::vec3>(v8::Local<v8::Value> value) {
    return value->IsObject();
}

template <>
bool isMatchCpp<glm::vec2>(v8::Local<v8::Value> value) {
    return value->IsObject();
}

template <>
bool isMatchCpp<glm::quat>(v8::Local<v8::Value> value) {
    return value->IsObject();
}

} // flappy
