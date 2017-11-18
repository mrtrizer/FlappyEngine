#pragma once

#include <map>
#include <unordered_map>

#include <Manager.h>

#include <v8.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "V8JSManager.h"
#include "SharedPtrHolder.h"

namespace flappy {

using _Bool = bool;

// Type matchers

template <typename T>
struct isMatchCpp {
    static bool check(v8::Local<v8::Value>) {
        LOGE("Unknown type");
        return true;
    }
};

template <template<typename...> class T, typename ... Args>
struct isMatchCpp<T<Args...>> {
    static bool check(v8::Local<v8::Value> value) {
        LOGE("Array matcher is not implemented");
        return true;
    }
};

template <>
struct isMatchCpp<std::string> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsString();
    }
};

template <>
struct isMatchCpp<bool> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsBoolean();
    }
};

template <>
struct isMatchCpp<int> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsNumber();
    }
};

template <>
struct isMatchCpp<float> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsNumber();
    }
};

template <>
struct isMatchCpp<glm::vec3> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsObject()
                && value.As<v8::Object>()->Has(toV8Str("x"))
                && value.As<v8::Object>()->Has(toV8Str("y"))
                && value.As<v8::Object>()->Has(toV8Str("z"));
    }
};

template <>
struct isMatchCpp<glm::vec2> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsObject()
                && value.As<v8::Object>()->Has(toV8Str("x"))
                && value.As<v8::Object>()->Has(toV8Str("y"))
                && !value.As<v8::Object>()->Has(toV8Str("z"));
    }
};

template <>
struct isMatchCpp<glm::quat> {
    static bool check(v8::Local<v8::Value> value) {
        return value->IsObject()
                && value.As<v8::Object>()->Has(toV8Str("x"))
                && value.As<v8::Object>()->Has(toV8Str("y"))
                && value.As<v8::Object>()->Has(toV8Str("z"))
                && value.As<v8::Object>()->Has(toV8Str("w"));
    }
};



// Wrapper templates

template <typename T>
struct toV8 {
    static v8::Local<v8::Value> cast(const T&);
};

template <typename T>
struct toCpp {
    static T cast(v8::Local<v8::Value>);
};

// Arrays

template<typename T, typename SubT, typename ... Args>
static v8::Local<v8::Value> toV8Array(T iterative) {
    auto array = v8::Array::New(v8::Isolate::GetCurrent(), iterative.size());
    int index = 0;
    for (auto iter = iterative.begin(); iter != iterative.end(); iter++) {
        (void)array->Set(v8::Isolate::GetCurrent()->GetCurrentContext(), index, toV8<SubT>::cast(*iter));
        index++;
    }
    return array;
}

template<template<typename ...> class T, typename ... Args>
struct toV8<T<Args...>> {
    static v8::Local<v8::Value> cast(T<Args...> iterative) {
        return toV8Array<T<Args...>, Args...>(iterative);
    }
};

template<typename T, typename SubT, typename ... Args>
static T arrayToCpp(v8::Local<v8::Value> value) {
    auto jsArray = value.As<v8::Array>();
    T cppIterative(jsArray->Length());
    for (int i = 0; i < jsArray->Length(); i++) {
        cppIterative[i] = toCpp<SubT>::cast(jsArray->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), i).ToLocalChecked());
    }
    return cppIterative;
}

template<template<typename ...> class T, typename ... Args>
struct toCpp<T<Args...>> {
    static T<Args...> cast(v8::Local<v8::Value> value) {
        return arrayToCpp<T<Args...>, Args...>(value);
    }
};

// Dictionaries

template<typename DictT, typename ValueT>
static v8::Local<v8::Value> toV8Dict(DictT iterative) {
    auto jsObj = v8::Object::New(v8::Isolate::GetCurrent());
    for (auto iter = iterative.begin(); iter != iterative.end(); iter++) {
        jsObj->Set(toV8Str(iter->first), toV8<ValueT>::cast(iter->second));
    }
    return jsObj;
}

template<typename ValueT>
struct toV8<std::map<std::string, ValueT>> {
    static v8::Local<v8::Value> cast(std::map<std::string, ValueT> iterative) {
        return toV8Dict<std::map<std::string, ValueT>, ValueT>(iterative);
    }
};

template<typename ValueT>
struct toV8<std::unordered_map<std::string, ValueT>> {
    static v8::Local<v8::Value> cast(std::unordered_map<std::string, ValueT> iterative) {
        return toV8Dict<std::unordered_map<std::string, ValueT>, ValueT>(iterative);
    }
};

template <typename DictT, typename ValueT>
static DictT toCppDict(v8::Local<v8::Value> value) {
    DictT cppDict;
    auto jsObj = value.As<v8::Object>();
    auto properties = jsObj->GetPropertyNames();
    for (int i = 0; i < properties->Length(); i++) {
        auto jsKey = properties->Get(i);
        auto key = std::string(*v8::String::Utf8Value(jsKey));
        cppDict[key] = toCpp<ValueT>::cast(jsObj->Get(v8::Isolate::GetCurrent()->GetCurrentContext(), jsKey));
    }
    return cppDict;
}

template<typename ValueT>
struct toCpp<std::map<std::string, ValueT>> {
    static std::map<std::string, ValueT> cast(v8::Local<v8::Value> value) {
        return toCppDict<std::map<std::string, ValueT>, ValueT>(value);
    }
};

template<typename ValueT>
struct toCpp<std::unordered_map<std::string, ValueT>> {
    static std::unordered_map<std::string, ValueT> cast(v8::Local<v8::Value> value) {
        return toCppDict<std::unordered_map<std::string, ValueT>, ValueT>(value);
    }
};

// Classes

void v8DestroyHolder(const v8::WeakCallbackInfo<CppObjectHolder> &data);

template<typename T>
struct toV8<std::shared_ptr<T>> {
    static v8::Local<v8::Value> cast(const std::shared_ptr<T>& value) {
        auto ptr = new SharedPtrHolder<T>(value);
        v8::Local<v8::External> jsPtr = v8::External::New(v8::Isolate::GetCurrent(), ptr);
        v8::UniquePersistent<v8::External> external(v8::Isolate::GetCurrent(), jsPtr);
        external.SetWeak<CppObjectHolder>(ptr, v8DestroyHolder, v8::WeakCallbackType::kParameter);
        persistentHolder.push_back(std::move(external));

        auto wrapperdObject = wrapperMap.get<T>().wrapper(value.get());
        wrapperdObject->SetInternalField(0, jsPtr);
        return wrapperdObject;
    }
};

template<typename T>
struct toCpp<std::shared_ptr<T>> {
    static std::shared_ptr<T> cast(v8::Local<v8::Value> value) {
        v8::Local<v8::External> internal = value.As<v8::Object>()->GetInternalField(0).As<v8::External>();
        auto sharedPtrHolder = static_cast<SharedPtrHolder<T>*>(internal->Value());
        return sharedPtrHolder->sharedPtr();
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
struct toV8<const char*> {
    static v8::Local<v8::Value> cast(const char* value) {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), value);
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
