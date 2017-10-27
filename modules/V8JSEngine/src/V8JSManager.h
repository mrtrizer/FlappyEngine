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

private:
    v8::Platform* m_platform;
    v8::Isolate* m_isolate;
    v8::ArrayBuffer::Allocator* m_arrayBufferAllocator;

    static ComponentBase* unwrapComponent(v8::Local<v8::Object> obj);
    static void getType(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info);

    v8::Local<v8::ObjectTemplate> makeComponentTemplate(v8::Isolate* isolate);
    v8::Local<v8::Object> wrapComponent(ComponentBase* component);

    void init();
    void deinit();
};

} // flappy
