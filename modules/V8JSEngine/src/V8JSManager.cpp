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

static Local<String> toV8Str(std::string stdStr, Isolate* isolate) {
    auto str = String::NewFromUtf8(
                isolate,
                stdStr.c_str(),
                NewStringType::kNormal,
                stdStr.length()).ToLocalChecked();
    return str;
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
    auto path = toV8Str(component->componentId().name(), info.GetIsolate());

    // Wrap the result in a JavaScript string and return it.
    info.GetReturnValue().Set(path);
}

Local<ObjectTemplate> V8JSManager::makeComponentTemplate(Isolate* isolate) {
    EscapableHandleScope handle_scope(isolate);

    Local<ObjectTemplate> result = ObjectTemplate::New(isolate);
    result->SetInternalFieldCount(1);

    // Add accessors for each of the fields of the request.
    result->SetAccessor(toV8Str("type", m_isolate), getType);

    // Again, return the result through the current handle scope.
    return handle_scope.Escape(result);
}


Local<Object> V8JSManager::wrapComponent(ComponentBase* component) {

    EscapableHandleScope handle_scope(m_isolate);
    v8::Local <v8::Context> context = v8::Local <v8::Context>::New (m_isolate, m_context);
    Context::Scope context_scope (context);

    Global<ObjectTemplate> componentTemplate;


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

static void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (args.Length() < 1)
      return;
  Local<Value> arg = args[0];
  String::Utf8Value value(arg);
  LOG("%s", *value);
}

void V8JSManager::runScript(Local<v8::Context>& context, std::string sourceStr) {
    // Create a string containing the JavaScript source code.
    v8::Local<v8::String> source = toV8Str(sourceStr, m_isolate);

    // Compile the source code.
    v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

    { // run
        TryCatch trycatch(m_isolate);

        MaybeLocal<Value> result = script->Run(context);

        if (result.IsEmpty()) {
          Local<Value> exception = trycatch.Exception();
          String::Utf8Value exception_str(exception);
          LOGE("Exception: %s\n", *exception_str);
        }

    }
}

void V8JSManager::callMethod(Local<Object> jsObject, std::string name, std::vector<Local<Value>> args) {
    HandleScope handleScope(m_isolate);
    v8::Local <v8::Context> context = v8::Local <v8::Context>::New (m_isolate, m_context);
    Context::Scope context_scope (context);

    Local<String> methodeName = toV8Str(name, m_isolate);
    Local<Value> updateVal;
    jsObject->Get(methodeName);
    auto detected = jsObject->Get(context, methodeName).ToLocal(&updateVal);
    if (!detected || !updateVal->IsFunction()) {
        LOGE("%s is not a function", name.c_str());
    }

    Local<Function> updateFun = Local<Function>::Cast(updateVal);

    TryCatch trycatch(m_isolate);

    auto result = updateFun->Call(context, context->Global(), args.size(), args.data());

    if (result.IsEmpty()) {
      Local<Value> exception = trycatch.Exception();
      String::Utf8Value exception_str(exception);
      LOGE("Exception: %s\n", *exception_str);
    }
}

MaybeLocal<Value> V8JSManager::callFunction(Local<v8::Context>& context, std::string name, std::vector<Local<Value>> args) {
    // The script compiled and ran correctly.  Now we fetch out the
    // Process function from the global object.
    Local<String> updateName = toV8Str(name, m_isolate);
    Local<Value> updateVal;
    auto detected = context->Global()->Get(context, updateName).ToLocal(&updateVal);
    if (!detected || !updateVal->IsFunction()) {
        LOGE("%s is not a function", name.c_str());
    }

    Local<Function> updateFun = Local<Function>::Cast(updateVal);

    TryCatch trycatch(m_isolate);

    auto result = updateFun->Call(context, context->Global(), args.size(), args.data());

    if (result.IsEmpty()) {
      Local<Value> exception = trycatch.Exception();
      String::Utf8Value exception_str(exception);
      LOGE("Exception: %s\n", *exception_str);
    }

    return result;
}

UniquePersistent<Object> V8JSManager::runJSComponent(std::string script) {
    HandleScope handleScope(m_isolate);

    v8::Local <v8::Context> context = v8::Local <v8::Context>::New (m_isolate, m_context);
    Context::Scope context_scope (context);

    runScript(context, script);
    auto jsObject = callFunction(context, "constructJsObject", {});
    if (jsObject.IsEmpty()) {
        LOGE("Result is empty");
    }
    Local<Value> jsObjectLocal = jsObject.ToLocalChecked();

    auto jsComponent = wrapComponent(this);

    return UniquePersistent<Object>(m_isolate, Local<Object>::Cast(jsObjectLocal));
}

void V8JSManager::assignComponentWrapper(Local<Object> jsObject, SafePtr<ComponentBase> component) {
    auto wrapped = wrapComponent(component->shared_from_this().get());
    callMethod(jsObject, "setWrapper", {wrapped});
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
        m_isolateScope = new v8::Isolate::Scope(m_isolate);
        HandleScope handleScope(m_isolate);

        Local<ObjectTemplate> global = ObjectTemplate::New(m_isolate);
        global->Set(toV8Str("log", m_isolate), FunctionTemplate::New(m_isolate, log));

        auto context = v8::Context::New(m_isolate, nullptr, global);

        // Create a new context.
        m_context = UniquePersistent<Context>(m_isolate, context);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope context_scope(context);

        runScript(context,
                    "class Component {"
                    "    constructor() {"
                    "        log('Component constructor');"
                    "    }"
                    "    setWrapper(wrapper) {"
                    "        this.wrapper = wrapper;"
                    "        log(this.wrapper.type);"
                    "    }"
                    "}"
                  );
    }

}

void V8JSManager::deinit() {
    // Dispose the isolate and tear down V8.
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete m_isolateScope;
    delete m_platform;
    delete m_arrayBufferAllocator;
}

} // flappy
