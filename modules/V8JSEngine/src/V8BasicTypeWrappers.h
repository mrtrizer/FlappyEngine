#pragma once

#include <map>
#include <unordered_map>

#include <Manager.h>

#include <v8.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "V8JSManager.h"

namespace flappy {

using _Bool = bool;

template <typename T>
struct toV8 {
    static v8::Local<v8::Value> cast(const T&);
};

template <typename T>
struct toCpp {
    static T cast(v8::Local<v8::Value>);
};

// Arrays

template<template<typename, typename> class X, typename T1, typename T2>
struct toV8<X<T1, T2>> {
    static v8::Local<v8::Value> cast(X<T1, T2> iterative) {
        return v8::Array::New(v8::Isolate::GetCurrent(), iterative.size());
    }
};

template<template<typename, typename, typename, typename> class X, typename T1, typename T2, typename T3, typename T4>
struct toV8<X<T1, T2, T3, T4>> {
    static v8::Local<v8::Value> cast(X<T1, T2, T3, T4> iterative) {
        return v8::Array::New(v8::Isolate::GetCurrent(), iterative.size());
    }
};

template<template<typename> class X, typename T>
struct toCpp<X<T>> {
    X<T> cast(v8::Local<v8::Value> value) {
        return X<T>();
    }
};

template<template<typename, typename> class X, typename T1, typename T2>
struct toCpp<X<T1, T2>> {
    static X<T1, T2> cast(v8::Local<v8::Value> value) {
        return X<T1, T2>();
    }
};

template<template<typename, typename, typename, typename> class X, typename T1, typename T2, typename T3, typename T4>
struct toCpp<X<T1, T2, T3, T4>> {
    static X<T1, T2, T3, T4> cast(v8::Local<v8::Value> value) {
        return X<T1, T2, T3, T4>();
    }
};

// Dictionaries

template<typename T1, typename T2>
struct toV8<std::map<T1, T2>> {
    static v8::Local<v8::Value> cast(std::map<T1, T2> iterative) {
        return v8::Object::New(v8::Isolate::GetCurrent());
    }
};

template<typename T1, typename T2>
struct toV8<std::unordered_map<T1, T2>> {
    static v8::Local<v8::Value> cast(std::unordered_map<T1, T2> iterative) {
        return v8::Object::New(v8::Isolate::GetCurrent());
    }
};


template<typename T1, typename T2>
struct toCpp<std::map<T1, T2>> {
    static std::map<T1, T2> cast(v8::Local<v8::Value> value) {
        return std::map<T1, T2>();
    }
};

template<typename T1, typename T2>
struct toCpp<std::unordered_map<T1, T2>> {
    static std::unordered_map<T1, T2> cast(v8::Local<v8::Value> value) {
        return std::unordered_map<T1, T2>();
    }
};

// Simple types

template <>
struct toV8<std::string> {
    static v8::Local<v8::Value> cast(const std::string& value) {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), value.c_str());
    }
};

template <>
struct toV8<bool> {
    static v8::Local<v8::Value> cast(const bool& value) {
        return v8::Boolean::New(v8::Isolate::GetCurrent(), value);
    }
};

template <>
struct toV8<int> {
    static v8::Local<v8::Value> cast(const int& value) {
        return v8::Number::New(v8::Isolate::GetCurrent(), value);
    }
};

template <>
struct toV8<float> {
    static v8::Local<v8::Value> cast(const float& value) {
        return v8::Number::New(v8::Isolate::GetCurrent(), value);
    }
};

template <>
struct toV8<glm::vec3> {
    static v8::Local<v8::Value> cast(const glm::vec3& value) {
        v8::Local<v8::Object> vec3Obj = v8::Object::New(v8::Isolate::GetCurrent());
        auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
        (void)vec3Obj->Set(context, toV8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), value.x));
        (void)vec3Obj->Set(context, toV8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), value.y));
        (void)vec3Obj->Set(context, toV8Str("z"), v8::Number::New(v8::Isolate::GetCurrent(), value.z));
        return vec3Obj;
    }
};

template <>
struct toV8<glm::vec2> {
    static v8::Local<v8::Value> cast(const glm::vec2& value) {
        v8::Local<v8::Object> vec2Obj = v8::Object::New(v8::Isolate::GetCurrent());
        auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
        (void)vec2Obj->Set(context, toV8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), value.x));
        (void)vec2Obj->Set(context, toV8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), value.y));
        return vec2Obj;
    }
};

template <>
struct toV8<glm::quat> {
    static v8::Local<v8::Value> cast(const glm::quat& value){
        v8::Local<v8::Object> quadObj = v8::Object::New(v8::Isolate::GetCurrent());
        auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
        (void)quadObj->Set(context, toV8Str("x"), v8::Number::New(v8::Isolate::GetCurrent(), value.x));
        (void)quadObj->Set(context, toV8Str("y"), v8::Number::New(v8::Isolate::GetCurrent(), value.y));
        (void)quadObj->Set(context, toV8Str("z"), v8::Number::New(v8::Isolate::GetCurrent(), value.z));
        (void)quadObj->Set(context, toV8Str("w"), v8::Number::New(v8::Isolate::GetCurrent(), value.w));
        return quadObj;
    }
};


template <>
struct toCpp<std::string> {
    static std::string cast(v8::Local<v8::Value> value) {
        return std::string(*v8::String::Utf8Value(value));
    }
};

template <>
struct toCpp<bool> {
    static bool cast(v8::Local<v8::Value> value) {
        return value.As<v8::Boolean>()->Value();
    }
};

template <>
struct toCpp<int> {
    static int cast(v8::Local<v8::Value> value) {
        return int(value.As<v8::Number>()->Value());
    }
};

template <>
struct toCpp<float> {
    static float cast(v8::Local<v8::Value> value) {
        return float(value.As<v8::Number>()->Value());
    }
};

template <>
struct toCpp<glm::vec3> {
    static glm::vec3 cast(v8::Local<v8::Value> value) {
        v8::Local<v8::Object> vec3Object = value.As<v8::Object>();
        auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
        float x = vec3Object->Get(context, toV8Str("x")).ToLocalChecked().As<v8::Number>()->Value();
        float y = vec3Object->Get(context, toV8Str("y")).ToLocalChecked().As<v8::Number>()->Value();
        float z = vec3Object->Get(context, toV8Str("z")).ToLocalChecked().As<v8::Number>()->Value();
        return glm::vec3(x, y, z);
    }
};

template <>
struct toCpp<glm::vec2> {
    static glm::vec2 cast(v8::Local<v8::Value> value) {
        v8::Local<v8::Object> vec2Object = value.As<v8::Object>();
        auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
        float x = vec2Object->Get(context, toV8Str("x")).ToLocalChecked().As<v8::Number>()->Value();
        float y = vec2Object->Get(context, toV8Str("y")).ToLocalChecked().As<v8::Number>()->Value();
        return glm::vec2(x, y);
    }
};

template <>
struct toCpp<glm::quat> {
    static glm::quat cast(v8::Local<v8::Value> value) {
        v8::Local<v8::Object> quatObject = value.As<v8::Object>();
        auto context = v8::Isolate::GetCurrent()->GetCurrentContext();
        float x = quatObject->Get(context, toV8Str("x")).ToLocalChecked().As<v8::Number>()->Value();
        float y = quatObject->Get(context, toV8Str("y")).ToLocalChecked().As<v8::Number>()->Value();
        float z = quatObject->Get(context, toV8Str("z")).ToLocalChecked().As<v8::Number>()->Value();
        float w = quatObject->Get(context, toV8Str("w")).ToLocalChecked().As<v8::Number>()->Value();
        return glm::quat(x, y, z, w);
    }
};



} // flappy
