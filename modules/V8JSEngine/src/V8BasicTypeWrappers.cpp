#include "V8BasicTypeWrappers.h"

#include "V8JSManager.h"

namespace flappy {

using namespace v8;

template <>
v8::Local<v8::Value> toV8<std::string>(const std::string& value) {
    return String::NewFromUtf8(Isolate::GetCurrent(), value.c_str());
}

template <>
v8::Local<v8::Value> toV8<bool>(const bool& value) {
    return Boolean::New(Isolate::GetCurrent(), value);
}

template <>
v8::Local<v8::Value> toV8<int>(const int& value) {
    return Number::New(Isolate::GetCurrent(), value);
}

template <>
v8::Local<v8::Value> toV8<float>(const float& value) {
    return Number::New(Isolate::GetCurrent(), value);
}

template <>
v8::Local<v8::Value> toV8<glm::vec3>(const glm::vec3& value) {
    Local<Object> vec3Obj = Object::New(Isolate::GetCurrent());
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    (void)vec3Obj->Set(context, toV8Str("x"), Number::New(Isolate::GetCurrent(), value.x));
    (void)vec3Obj->Set(context, toV8Str("y"), Number::New(Isolate::GetCurrent(), value.y));
    (void)vec3Obj->Set(context, toV8Str("z"), Number::New(Isolate::GetCurrent(), value.z));
    return vec3Obj;
}

template <>
v8::Local<v8::Value> toV8<glm::vec2>(const glm::vec2& value) {
    Local<Object> vec2Obj = Object::New(Isolate::GetCurrent());
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    (void)vec2Obj->Set(context, toV8Str("x"), Number::New(Isolate::GetCurrent(), value.x));
    (void)vec2Obj->Set(context, toV8Str("y"), Number::New(Isolate::GetCurrent(), value.y));
    return vec2Obj;
}

template <>
v8::Local<v8::Value> toV8<glm::quat>(const glm::quat& value){
    Local<Object> quadObj = Object::New(Isolate::GetCurrent());
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    (void)quadObj->Set(context, toV8Str("x"), Number::New(Isolate::GetCurrent(), value.x));
    (void)quadObj->Set(context, toV8Str("y"), Number::New(Isolate::GetCurrent(), value.y));
    (void)quadObj->Set(context, toV8Str("z"), Number::New(Isolate::GetCurrent(), value.z));
    (void)quadObj->Set(context, toV8Str("w"), Number::New(Isolate::GetCurrent(), value.w));
    return quadObj;
}


template <>
std::string toCpp<std::string>(v8::Local<v8::Value> value) {
    return std::string(*String::Utf8Value(value));
}

template <>
bool toCpp<bool>(v8::Local<v8::Value> value) {
    return value.As<Boolean>()->Value();
}

template <>
int toCpp<int>(v8::Local<v8::Value> value) {
    return int(value.As<Number>()->Value());
}

template <>
float toCpp<float>(v8::Local<v8::Value> value) {
    return float(value.As<Number>()->Value());
}

template <>
glm::vec3 toCpp<glm::vec3>(v8::Local<v8::Value> value) {
    Local<Object> vec3Object = value.As<Object>();
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    float x = vec3Object->Get(context, toV8Str("x")).ToLocalChecked().As<Number>()->Value();
    float y = vec3Object->Get(context, toV8Str("y")).ToLocalChecked().As<Number>()->Value();
    float z = vec3Object->Get(context, toV8Str("z")).ToLocalChecked().As<Number>()->Value();
    return glm::vec3(x, y, z);
}

template <>
glm::vec2 toCpp<glm::vec2>(v8::Local<v8::Value> value) {
    Local<Object> vec2Object = value.As<Object>();
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    float x = vec2Object->Get(context, toV8Str("x")).ToLocalChecked().As<Number>()->Value();
    float y = vec2Object->Get(context, toV8Str("y")).ToLocalChecked().As<Number>()->Value();
    return glm::vec2(x, y);
}

template <>
glm::quat toCpp<glm::quat>(v8::Local<v8::Value> value) {
    Local<Object> quatObject = value.As<Object>();
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    float x = quatObject->Get(context, toV8Str("x")).ToLocalChecked().As<Number>()->Value();
    float y = quatObject->Get(context, toV8Str("y")).ToLocalChecked().As<Number>()->Value();
    float z = quatObject->Get(context, toV8Str("z")).ToLocalChecked().As<Number>()->Value();
    float w = quatObject->Get(context, toV8Str("w")).ToLocalChecked().As<Number>()->Value();
    return glm::quat(x, y, z, w);
}

} // flappy
