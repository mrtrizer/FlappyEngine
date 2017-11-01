#include "V8BasicTypeWrappers.h"

#include "V8JSManager.h"

namespace flappy {

using namespace v8;

v8::Local<v8::Value> std__string_to_v8(std::string value) {
    return String::NewFromUtf8(Isolate::GetCurrent(), value.c_str());
}

v8::Local<v8::Value> int_to_v8(int value) {
    return Number::New(Isolate::GetCurrent(), value);
}

v8::Local<v8::Value> float_to_v8(float value) {
    return Number::New(Isolate::GetCurrent(), value);
}

v8::Local<v8::Value> _Bool_to_v8(bool value) {
    return Boolean::New(Isolate::GetCurrent(), value);
}

v8::Local<v8::Value> glm__vec3_to_v8(glm::vec3 value) {
    Local<Object> vec3Obj = Object::New(Isolate::GetCurrent());
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    (void)vec3Obj->Set(context, toV8Str("x"), Number::New(Isolate::GetCurrent(), value.x));
    (void)vec3Obj->Set(context, toV8Str("y"), Number::New(Isolate::GetCurrent(), value.y));
    (void)vec3Obj->Set(context, toV8Str("z"), Number::New(Isolate::GetCurrent(), value.z));
    return vec3Obj;
}

v8::Local<v8::Value> glm__vec2_to_v8(glm::vec2 value) {
    Local<Object> vec2Obj = Object::New(Isolate::GetCurrent());
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    (void)vec2Obj->Set(context, toV8Str("x"), Number::New(Isolate::GetCurrent(), value.x));
    (void)vec2Obj->Set(context, toV8Str("y"), Number::New(Isolate::GetCurrent(), value.y));
    return vec2Obj;
}

v8::Local<v8::Value> glm__quat_to_v8(glm::quat value) {
    Local<Object> quadObj = Object::New(Isolate::GetCurrent());
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    (void)quadObj->Set(context, toV8Str("x"), Number::New(Isolate::GetCurrent(), value.x));
    (void)quadObj->Set(context, toV8Str("y"), Number::New(Isolate::GetCurrent(), value.y));
    (void)quadObj->Set(context, toV8Str("z"), Number::New(Isolate::GetCurrent(), value.z));
    (void)quadObj->Set(context, toV8Str("w"), Number::New(Isolate::GetCurrent(), value.w));
    return quadObj;
}


std::string v8_to_std__string(v8::Local<v8::Value> value) {
    return std::string(*String::Utf8Value(value));
}

int v8_to_int(v8::Local<v8::Value> value) {
    return int(value.As<Number>()->Value());
}

float v8_to_float(v8::Local<v8::Value> value) {
    return float(value.As<Number>()->Value());
}

bool v8_to__Bool(v8::Local<v8::Value> value) {
    return value.As<Boolean>()->Value();
}

glm::vec3 v8_to_glm__vec3(v8::Local<v8::Value> value) {
    Local<Object> vec3Object = value.As<Object>();
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    float x = vec3Object->Get(context, toV8Str("x")).ToLocalChecked().As<Number>()->Value();
    float y = vec3Object->Get(context, toV8Str("y")).ToLocalChecked().As<Number>()->Value();
    float z = vec3Object->Get(context, toV8Str("z")).ToLocalChecked().As<Number>()->Value();
    return glm::vec3(x, y, z);
}

glm::vec2 v8_to_glm__vec2(v8::Local<v8::Value> value) {
    Local<Object> vec2Object = value.As<Object>();
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    float x = vec2Object->Get(context, toV8Str("x")).ToLocalChecked().As<Number>()->Value();
    float y = vec2Object->Get(context, toV8Str("y")).ToLocalChecked().As<Number>()->Value();
    return glm::vec2(x, y);
}

glm::quat glm__quat_to_v8(v8::Local<v8::Value> value) {
    Local<Object> quatObject = value.As<Object>();
    auto context = Isolate::GetCurrent()->GetCurrentContext();
    float x = quatObject->Get(context, toV8Str("x")).ToLocalChecked().As<Number>()->Value();
    float y = quatObject->Get(context, toV8Str("y")).ToLocalChecked().As<Number>()->Value();
    float z = quatObject->Get(context, toV8Str("z")).ToLocalChecked().As<Number>()->Value();
    float w = quatObject->Get(context, toV8Str("w")).ToLocalChecked().As<Number>()->Value();
    return glm::quat(x, y, z, w);
}

} // flappy
