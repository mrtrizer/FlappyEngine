#pragma once

#include <Manager.h>

#include <v8.h>

namespace flappy {

class TouchComponent;

class V8JSManager: public Manager<V8JSManager>
{
public:
    V8JSManager();

private:
    v8::Platform* m_platform;
    v8::Isolate* m_isolate;
    v8::ArrayBuffer::Allocator* m_arrayBufferAllocator;

    void init();
    void deinit();

};

} // flappy
