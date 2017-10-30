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

// TODO: Remove isolate param
static Local<String> toV8Str(std::string stdStr) {
    auto str = String::NewFromUtf8(
                Isolate::GetCurrent(),
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

static ComponentBase* unwrapComponent(Local<Object> obj) {
  Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<ComponentBase*>(ptr);
}

static void Component_getType(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent(info.Holder());
    auto path = toV8Str(component->componentId().name());
    info.GetReturnValue().Set(path);
}

static void Component_isInitialized(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent(info.Holder());
    info.GetReturnValue().Set(component->isInitialized());
}

static void Component_active(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent(info.Holder());
    info.GetReturnValue().Set(component->active());
}

static void Component_setActive(Local<String>, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    ComponentBase* component = unwrapComponent(info.Holder());
    component->setActive(value->BooleanValue());
}

static void Component_testFunc_callback(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    void* ptr = field->Value();
    auto component =  static_cast<ComponentBase*>(ptr);
    auto name = toV8Str(component->componentId().name());
    info.GetReturnValue().Set(name);
}

Local<Object> V8JSManager::wrapComponent(ComponentBase* component) {

    EscapableHandleScope handle_scope(m_isolate);
    v8::Local <v8::Context> context = v8::Local <v8::Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    Local<External> componentJSPtr = External::New(m_isolate, component);

    v8::Local<v8::FunctionTemplate> funcTemplate = v8::FunctionTemplate::New(m_isolate);
    v8::Local<v8::Template> prototype = funcTemplate->PrototypeTemplate();
    prototype->Set(toV8Str("testFunc"), v8::FunctionTemplate::New(m_isolate, Component_testFunc_callback, componentJSPtr));
    v8::Local<v8::ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();

    componentTemplate->SetInternalFieldCount(1);

    // Add accessors for each of the fields of the request.
    componentTemplate->SetAccessor(toV8Str("type"), Component_getType);
    componentTemplate->SetAccessor(toV8Str("initialized"), Component_isInitialized);
    componentTemplate->SetAccessor(toV8Str("active"), Component_active, Component_setActive);

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(m_isolate, componentTemplate);

    Local<Object> result = templ->NewInstance(m_isolate->GetCurrentContext()).ToLocalChecked();

    result->SetInternalField(0, componentJSPtr);

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
    v8::Local<v8::String> source = toV8Str(sourceStr);

    { // run
        TryCatch trycatch(m_isolate);

        // Compile the source code.
        auto compileResult = v8::Script::Compile(context, source);

        if (compileResult.IsEmpty()) {
            Local<Value> exception = trycatch.Exception();
            Local<Message> message = trycatch.Message();
            int lineNumberInt = message->GetLineNumber();
            String::Utf8Value exceptionStr(exception);
            LOGE("Exception: %s %d\n", *exceptionStr, lineNumberInt);
        }

        v8::Local<v8::Script> script = compileResult.ToLocalChecked();

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
    Context::Scope contextScope (context);

    Local<String> methodeName = toV8Str(name);
    Local<Value> updateVal;
    auto detected = jsObject->Get(context, methodeName).ToLocal(&updateVal);
    if (!detected || !updateVal->IsFunction()) {
        LOGE("%s is not a function", name.c_str());
    }

    Local<Function> updateFun = Local<Function>::Cast(updateVal);

    TryCatch trycatch(m_isolate);

    auto result = updateFun->Call(jsObject, args.size(), args.data());

    if (result.IsEmpty()) {
      Local<Value> exception = trycatch.Exception();
      String::Utf8Value exception_str(exception);
      LOGE("Exception: %s\n", *exception_str);
    }
}

MaybeLocal<Value> V8JSManager::callFunction(Local<v8::Context>& context, std::string name, std::vector<Local<Value>> args) {
    // The script compiled and ran correctly.  Now we fetch out the
    // Process function from the global object.
    Local<String> updateName = toV8Str(name);
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

UniquePersistent<Object> V8JSManager::runJSComponent(std::string script, SafePtr<ComponentBase> component) {
    HandleScope handleScope(m_isolate);

    v8::Local <v8::Context> context = v8::Local <v8::Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    runScript(context, script);
    auto wrapped = wrapComponent(component->shared_from_this().get());

    auto jsObject = callFunction(context, "constructJsObject", {wrapped});
    if (jsObject.IsEmpty()) {
        LOGE("Result is empty");
    }
    Local<Value> jsObjectLocal = jsObject.ToLocalChecked();

    return UniquePersistent<Object>(m_isolate, Local<Object>::Cast(jsObjectLocal));
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
        global->Set(toV8Str("log"), FunctionTemplate::New(m_isolate, log));

        auto context = v8::Context::New(m_isolate, nullptr, global);

        // Create a new context.
        m_context = UniquePersistent<Context>(m_isolate, context);

        // Enter the context for compiling and running the hello world script.
        v8::Context::Scope contextScope(context);

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
