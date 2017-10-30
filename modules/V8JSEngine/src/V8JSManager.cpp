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

class ArrayBufferAllocator : public ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

template <typename ObjT>
static ComponentBase* unwrapComponent(Local<Object> obj) {
  Local<External> field = Local<External>::Cast(obj->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<ObjT*>(ptr);
}

static void Entity_testFunc_callback(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    void* ptr = field->Value();
    auto name = toV8Str("EntityTest");
    info.GetReturnValue().Set(name);
}

Local<Object> V8JSManager::wrapEntity(Entity* entity) {

    EscapableHandleScope handle_scope(m_isolate);
    Local <Context> context = Local <Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    Local<External> jsPtr = External::New(m_isolate, entity);

    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(m_isolate);
    Local<Template> prototype = funcTemplate->PrototypeTemplate();
    prototype->Set(toV8Str("testFunc"), FunctionTemplate::New(m_isolate, Entity_testFunc_callback, jsPtr));

    Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();
    componentTemplate->SetInternalFieldCount(1);

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(m_isolate, componentTemplate);

    Local<Object> result = templ->NewInstance(m_isolate->GetCurrentContext()).ToLocalChecked();

    result->SetInternalField(0, jsPtr);

    return handle_scope.Escape(result);
}

static void Component_getType(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    auto path = toV8Str(component->componentId().name());
    info.GetReturnValue().Set(path);
}

static void Component_isInitialized(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    info.GetReturnValue().Set(component->isInitialized());
}

static void Component_active(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    info.GetReturnValue().Set(component->active());
}

static void Component_setActive(Local<String>, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
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
    Local <Context> context = Local <Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    Local<External> componentJSPtr = External::New(m_isolate, component);

    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(m_isolate);
    Local<Template> prototype = funcTemplate->PrototypeTemplate();
    prototype->Set(toV8Str("testFunc"), FunctionTemplate::New(m_isolate, Component_testFunc_callback, componentJSPtr));
    prototype->Set(toV8Str("entity"), wrapEntity(component->entity()));
    Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();

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

static void log(const FunctionCallbackInfo<Value>& args) {
  if (args.Length() < 1)
      return;
  Local<Value> arg = args[0];
  String::Utf8Value value(arg);
  LOG("%s", *value);
}

void V8JSManager::runScript(Local<Context>& context, std::string sourceStr) {
    // Create a string containing the JavaScript source code.
    Local<String> source = toV8Str(sourceStr);

    { // run
        TryCatch trycatch(m_isolate);

        // Compile the source code.
        auto compileResult = Script::Compile(context, source);

        if (compileResult.IsEmpty()) {
            Local<Value> exception = trycatch.Exception();
            Local<Message> message = trycatch.Message();
            int lineNumberInt = message->GetLineNumber();
            String::Utf8Value exceptionStr(exception);
            LOGE("Exception: %s %d\n", *exceptionStr, lineNumberInt);
        }

        Local<Script> script = compileResult.ToLocalChecked();

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
    Local <Context> context = Local <Context>::New (m_isolate, m_context);
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

MaybeLocal<Value> V8JSManager::callFunction(Local<Context>& context, std::string name, std::vector<Local<Value>> args) {
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

UniquePersistent<Object> V8JSManager::runJSComponent(std::string name, std::string script, SafePtr<ComponentBase> component) {
    HandleScope handleScope(m_isolate);

    auto extendedScript = Tools::format("\n"
                                        "function constructJsComponent(wrapper) {\n"
                                        "   let ComponentWrapper = function () {"
                                        "       log(this.initialized.toString());\n"
                                        "       this.testField = 10;\n"
                                        "   }\n"
                                        "\n"
                                        "   ComponentWrapper.prototype = wrapper;\n"
                                        "\n"
                                        "   class Component extends ComponentWrapper {\n"
                                        "       constructor() {\n"
                                        "           super();\n"
                                        "           log('Component constructor');"
                                        "           log(this.initialized.toString());\n"
                                        "       }\n"
                                        "   }\n"
                                        "   %s"
                                        "   log('constructJsObject start');"
                                        "   let testComponent = new %s();"
                                        "   log('constructJsObject end');"
                                        "   testComponent.update(100);"
                                        "   return testComponent;"
                                        "}", script.c_str(), name.c_str());

    Local <Context> context = Local <Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    runScript(context, extendedScript);
    auto wrapped = wrapComponent(component->shared_from_this().get());

    auto jsObject = callFunction(context, "constructJsComponent", {wrapped});
    if (jsObject.IsEmpty()) {
        LOGE("Result is empty");
    }
    Local<Value> jsObjectLocal = jsObject.ToLocalChecked();

    return UniquePersistent<Object>(m_isolate, Local<Object>::Cast(jsObjectLocal));
}

void V8JSManager::init() {
    // Initialize V8.
    m_platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(m_platform);
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    Isolate::CreateParams createParams;
    m_arrayBufferAllocator = new ArrayBufferAllocator;
    createParams.array_buffer_allocator = m_arrayBufferAllocator;
    m_isolate = Isolate::New(createParams);
    {
        m_isolateScope = new Isolate::Scope(m_isolate);
        HandleScope handleScope(m_isolate);

        Local<ObjectTemplate> global = ObjectTemplate::New(m_isolate);
        global->Set(toV8Str("log"), FunctionTemplate::New(m_isolate, log));

        auto context = Context::New(m_isolate, nullptr, global);

        // Create a new context.
        m_context = UniquePersistent<Context>(m_isolate, context);

        // Enter the context for compiling and running the hello world script.
        Context::Scope contextScope(context);

    }

}

void V8JSManager::deinit() {
    // Dispose the isolate and tear down V8.
    V8::Dispose();
    V8::ShutdownPlatform();
    delete m_isolateScope;
    delete m_platform;
    delete m_arrayBufferAllocator;
}

} // flappy
