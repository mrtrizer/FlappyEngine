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

    v8::UniquePersistent<v8::Object> runJSComponent(std::string script);
    void callMethod(v8::Local<v8::Object> jsObject, std::string name, std::vector<v8::Local<v8::Value>> args);
    void assignComponentWrapper(v8::Local<v8::Object> jsObject, SafePtr<ComponentBase> component);

private:
    v8::Platform* m_platform;
    v8::Isolate* m_isolate;
    v8::Isolate::Scope* m_isolateScope;
    v8::UniquePersistent<v8::Context> m_context;
    v8::ArrayBuffer::Allocator* m_arrayBufferAllocator;

    static ComponentBase* unwrapComponent(v8::Local<v8::Object> obj);
    static void getType(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

    v8::Local<v8::ObjectTemplate> makeComponentTemplate(v8::Isolate* isolate);
    v8::Local<v8::Object> wrapComponent(ComponentBase* component);

    void runScript(v8::Local<v8::Context>& context, std::string sourceStr);
    v8::MaybeLocal<v8::Value> callFunction(v8::Local<v8::Context>& context, std::string name, std::vector<v8::Local<v8::Value>> args);

    void init();
    void deinit();
};

} // flappy
