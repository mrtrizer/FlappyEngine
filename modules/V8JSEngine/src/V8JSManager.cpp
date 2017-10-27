#include "V8JSManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <ComponentBase.h>

namespace flappy {

using namespace v8;

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

ComponentBase* V8JSManager::unwrapComponent(Local<Object> obj) {
  Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<ComponentBase*>(ptr);
}

void V8JSManager::getType(Local<String> name, const PropertyCallbackInfo<Value>& info) {
    // Extract the C++ request object from the JavaScript wrapper.
    ComponentBase* component = unwrapComponent(info.Holder());

    // Fetch the path.
    const std::string& path = component->componentId().name();

    auto str = String::NewFromUtf8(
                info.GetIsolate(),
                path.c_str(),
                NewStringType::kNormal,
                path.length()).ToLocalChecked();

    // Wrap the result in a JavaScript string and return it.
    info.GetReturnValue().Set(str);
}

Local<ObjectTemplate> V8JSManager::makeComponentTemplate(Isolate* isolate) {
    EscapableHandleScope handle_scope(isolate);

    Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    // Add accessors for each of the fields of the request.
    auto getTypeName = String::NewFromUtf8(isolate, "type", NewStringType::kInternalized).ToLocalChecked();
    result->SetAccessor(getTypeName, getType);

    // Again, return the result through the current handle scope.
    return handle_scope.Escape(result);
}


Local<Object> V8JSManager::wrapComponent(ComponentBase* component) {

    Global<ObjectTemplate> componentTemplate;

    // Local scope for temporary handles.
    EscapableHandleScope handle_scope(m_isolate);

    // Fetch the template for creating JavaScript http request wrappers.
    // It only has to be created once, which we do on demand.
    Local<ObjectTemplate> rawComponentTemplate = makeComponentTemplate(m_isolate);
    componentTemplate.Reset(m_isolate, rawComponentTemplate);

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(m_isolate, componentTemplate);

    // Create an empty http request wrapper.
    Local<Object> result = templ->NewInstance(m_isolate->GetCurrentContext()).ToLocalChecked();

    // Wrap the raw C++ pointer in an External so it can be referenced
    // from within JavaScript.
    Local<External> request_ptr = External::New(m_isolate, component);

    // Store the request pointer in the JavaScript wrapper.
    result->SetInternalField(0, request_ptr);

    // Return the result through the current handle scope.  Since each
    // of these handles will go away when the handle scope is deleted
    // we need to call Close to let one, the result, escape into the
    // outer handle scope.
    return handle_scope.Escape(result);
}

static Local<String> toV8Str(std::string stdStr, Isolate* isolate) {
    auto str = String::NewFromUtf8(
                isolate,
                stdStr.c_str(),
                NewStringType::kNormal,
                stdStr.length()).ToLocalChecked();
    return str;
}

static void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
      return;
  Local<Value> arg = args[0];
  String::Utf8Value value(arg);
  LOG("%s", *value);
}

void V8JSManager::init() {
    // Initialize V8.
    m_platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(m_platform);
    v8::V8::Initialize();

    // Create a new Isolate and make it the current one.
    v8::Isolate::CreateParams createParams;
    m_arrayBufferAllocator = new ArrayBufferAllocator;
    createParams.array_buffer_allocator = m_arrayBufferAllocator;
    m_isolate = v8::Isolate::New(createParams);
    {
        v8::Isolate::Scope isolate_scope(m_isolate);

        // Create a stack-allocated handle scope.
        v8::HandleScope handle_scope(m_isolate);

        Local<ObjectTemplate> global = ObjectTemplate::New(m_isolate);
        global->Set(toV8Str("log", m_isolate), FunctionTemplate::New(m_isolate, log));

        // Create a new context.
        v8::Local<v8::Context> context = v8::Context::New(m_isolate, nullptr, global);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);


        // Create a string containing the JavaScript source code.
        v8::Local<v8::String> source = toV8Str(
                    "function update(component) {"
                    "    log('Update' + component.type);"
                    "}; "
                    "log('Test');", m_isolate);


        // Compile the source code.
        v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

        script->Run(context);
//        // Run the script to get the result.
//        v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();

//        // Convert the result to an UTF8 string and print it.
//        v8::String::Utf8Value utf8(result);

//        LOG("%s", *utf8);


        // The script compiled and ran correctly.  Now we fetch out the
        // Process function from the global object.
        Local<String> updateName = toV8Str("update", m_isolate);
        Local<Value> updateVal;
        auto detected = context->Global()->Get(context, updateName).ToLocal(&updateVal);
        if (!detected)
            LOGE("update is not detected");
        // If there is no Process function, or if it is not a function,
        // bail out
        if (!updateVal->IsFunction()) {
            LOGE("update is not a function");
        }

        Local<Function> updateFun = Local<Function>::Cast(updateVal);

        Global<Function> update;

        // Store the function in a Global handle, since we also want
        // that to remain after this call returns
        update.Reset(m_isolate, updateFun);

        Local<Object> jsComponent = wrapComponent(this);

        const int argc = 1;
        Local<Value> argv[argc] = {jsComponent};
        v8::Local<v8::Function> process = v8::Local<v8::Function>::New(m_isolate, update);
        process->Call(context, context->Global(), argc, argv);
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
