#pragma once

#include <Manager.h>

#include <v8.h>

namespace flappy {

class TouchComponent;
class ComponentBase;

class V8JSManager: public Manager<V8JSManager>
{
public:
    V8JSManager();

    v8::UniquePersistent<v8::Object> runJSComponent(std::string name, std::string script, SafePtr<ComponentBase> component);
    void callMethod(v8::Local<v8::Object> jsObject, std::string name, std::vector<v8::Local<v8::Value> > args);

private:
    v8::Platform* m_platform;
    v8::Isolate* m_isolate;
    v8::Isolate::Scope* m_isolateScope;
    v8::UniquePersistent<v8::Context> m_context;
    v8::ArrayBuffer::Allocator* m_arrayBufferAllocator;

    v8::Local<v8::Object> wrapEntity(Entity* entity);
    v8::Local<v8::Object> wrapComponent(ComponentBase* component);

    void runScript(v8::Local<v8::Context>& context, std::string sourceStr);
    v8::MaybeLocal<v8::Value> callFunction(v8::Local<v8::Context>& context, std::string name, std::vector<v8::Local<v8::Value>> args);

    void init();
    void deinit();
};

v8::Local<v8::String> toV8Str(std::string stdStr);

struct Wrapper {
    std::string name;
    std::function<v8::Local<v8::Object>(void*)> wrapper;
    std::function<v8::Local<v8::Function>()> createConstructor;
};

extern std::unordered_map<std::string, Wrapper> wrapperMap;
extern std::string jsInitCode;

} // flappy
