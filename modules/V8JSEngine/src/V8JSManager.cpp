#include "V8JSManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libplatform/libplatform.h>
#include <v8.h>

namespace flappy {

V8JSManager::V8JSManager()
{
    subscribe([this](InitEvent) { init(); });
    subscribe([this](DeinitEvent) { deinit(); });
}

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

void V8JSManager::init() {
    // Initialize V8.
    m_platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(m_platform);
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams createParams;
    m_arrayBufferAllocator = new ArrayBufferAllocator;
    createParams.array_buffer_allocator = m_arrayBufferAllocator;
    v8::Isolate* isolate = v8::Isolate::New(createParams);
    {
        v8::Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(isolate);

        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(isolate);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);

        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source =
          v8::String::NewFromUtf8(isolate, "'Hello' + ', World!'",
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();

        // Compile the source code.
        v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

        // Run the script to get the result.
        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

        // Convert the result to an UTF8 string and print it.
        v8::String::Utf8Value utf8(result);

        LOG("%s", *utf8);
    }
}

void V8JSManager::deinit() {
    // Dispose the isolate and tear down V8.
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    m_isolate->Dispose();
    delete m_platform;
    delete m_arrayBufferAllocator;
}

} // flappy
