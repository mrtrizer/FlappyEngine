#pragma once

#include <functional>

#include <v8.h>

#include <SafePtr.h>
#include <TypeMap.h>

namespace flappy {

v8::Local<v8::Context> currentContext();
v8::Local<v8::String> toV8Str(std::string stdStr);
v8::Local<v8::Private> toV8PrivateKey(std::string stdStr);

struct Wrapper {
    std::string name;
    std::function<v8::Local<v8::Object>(SafePtrBase&)> wrapper;
    std::function<v8::Local<v8::Function>()> createConstructor;
};

extern std::vector<v8::UniquePersistent<v8::External>> persistentHolder;
extern TypeMap<void, Wrapper> wrapperMap;

} // flappy
