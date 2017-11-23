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
    v8::Local<v8::Value> callMethod(v8::Local<v8::Object> jsObject, std::string name, std::vector<v8::Local<v8::Value> > args);
    v8::Local<v8::Value> getField(v8::Local<v8::Object> jsObject, std::string name);

    const v8::UniquePersistent<v8::Context>* context() const { return &m_context; }

private:
    v8::Platform* m_platform;
    v8::Isolate* m_isolate;
    v8::UniquePersistent<v8::Context> m_context;
    v8::ArrayBuffer::Allocator* m_arrayBufferAllocator;

    void runScript(v8::Local<v8::Context>& context, std::string sourceStr);
    v8::Local<v8::Value> callFunction(std::string name, std::vector<v8::Local<v8::Value>> args);

    void init();
    void deinit();
};

} // flappy
