#include "V8JSManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <Entity.h>
#include <ComponentBase.h>
#include <JSComponent.h>
#include <V8ComponentWrapper.h>

namespace flappy {

using namespace v8;

V8JSManager::V8JSManager()
{
    subscribe([this](InitEvent) { init(); });
    subscribe([this](DeinitEvent) { deinit(); });
}

Local<Context> currentContext() {
    return Isolate::GetCurrent()->GetCurrentContext();
}

Local<String> toV8Str(std::string stdStr) {
    auto str = String::NewFromUtf8(
                Isolate::GetCurrent(),
                stdStr.c_str(),
                NewStringType::kNormal,
                stdStr.length()).ToLocalChecked();
    return str;
}

Local<Private> toV8PrivateKey(std::string stdStr) {
    auto str = String::NewFromUtf8(
                Isolate::GetCurrent(),
                stdStr.c_str(),
                NewStringType::kNormal,
                stdStr.length()).ToLocalChecked();
    auto privateKey = Private::ForApi(Isolate::GetCurrent(), str);
    return privateKey;
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

TypeMap<void, Wrapper> wrapperMap;
std::vector<v8::UniquePersistent<v8::External>> persistentHolder;

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
        } else {
            Local<Script> script = compileResult.ToLocalChecked();

            MaybeLocal<Value> result = script->Run(context);

            if (result.IsEmpty()) {
              Local<Value> exception = trycatch.Exception();
              String::Utf8Value exception_str(exception);
              LOGE("Exception: %s\n", *exception_str);
            }
        }

    }
}

Local<Value> V8JSManager::callMethod(Local<Object> jsObject, std::string name, std::vector<v8::Local<Value> > args) {
    EscapableHandleScope handleScope(m_isolate);
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

    auto result = updateFun->Call(context, jsObject, args.size(), args.data());

    if (result.IsEmpty()) {
        Local<Value> exception = trycatch.Exception();
        String::Utf8Value exception_str(exception);
        LOGE("Exception: %s\n", *exception_str);
    }

    return handleScope.Escape(result.ToLocalChecked());
}

v8::Local<v8::Value> V8JSManager::getField(v8::Local<v8::Object> jsObject, std::string name) {
    EscapableHandleScope handleScope(m_isolate);
    Local <Context> context = Local <Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    Local<String> methodeName = toV8Str(name);
    Local<Value> fieldValue;
    auto detected = jsObject->Get(context, methodeName).ToLocal(&fieldValue);
    if (!detected) {
        LOGE("%s is not a field", name.c_str());
    }

    return handleScope.Escape(fieldValue);
}

void V8JSManager::registerWrappers(const TypeMap<void, Wrapper>& wrappers)
{
    for (auto pair : wrappers) {
        auto counterIter = m_counterMap.find(pair.second.name);
        if (counterIter == m_counterMap.end()) {
            defineWrapperHelpers(pair.second);
            wrapperMap.setById(pair.first, pair.second);
            m_counterMap[pair.second.name] = 1;
        }
        registerWrapper(pair.second);
    }
}

void V8JSManager::unregisterWrappers(const TypeMap<void, Wrapper> &wrappers)
{
    for (auto pair : wrappers) {
        auto counterIter = m_counterMap.find(pair.second.name);
        if (counterIter != m_counterMap.end()) {
            if (counterIter->second > 0) {
                unregisterWrapper(pair.second.name);
                counterIter->second--;
            }
        }
    }
}

void V8JSManager::defineWrapperHelpers(const Wrapper& wrapper)
{
    HandleScope handleScope(m_isolate);
    auto context = m_context.Get(m_isolate);
    Context::Scope contextScope (context);

    auto className = wrapper.name;
    std::stringstream ss;
    ss << "let " << className << " = {};";
    ss << "function register" << className << "(func) {";
    ss << "    " << className << " = func;";
    ss << "    log('" << className << " is registered')";
    ss << "}";
    ss << "function unregister" << className << "() {";
    ss << "    " << className << " = undefined;";
    ss << "    log('" << className << " is unregistered')";
    ss << "}";
    runScript(context, ss.str());
}

void V8JSManager::registerWrapper(const Wrapper& wrapper)
{
    HandleScope handleScope(m_isolate);
    auto context = m_context.Get(m_isolate);
    Context::Scope contextScope (context);

    auto constructor = wrapper.createConstructor();
    callFunction("register" + wrapper.name, {constructor});
}

void V8JSManager::unregisterWrapper(const std::string& className)
{
    HandleScope handleScope(m_isolate);
    auto context = m_context.Get(m_isolate);
    Context::Scope contextScope (context);

    callFunction("unregister" + className);
}

Local<Value> V8JSManager::callFunction(std::string name, std::vector<Local<Value>> args) {
    EscapableHandleScope handleScope(m_isolate);
    Local <Context> context = Local <Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    Local<String> functionName = toV8Str(name);
    Local<Value> functionValue;
    auto detected = context->Global()->Get(context, functionName).ToLocal(&functionValue);
    if (!detected || !functionValue->IsFunction()) {
        LOGE("%s is not a function", name.c_str());
    }

    Local<Function> updateFun = Local<Function>::Cast(functionValue);

    TryCatch trycatch(m_isolate);

    auto result = updateFun->Call(context, context->Global(), args.size(), args.data());

    if (result.IsEmpty()) {
        Local<Value> exception = trycatch.Exception();
        String::Utf8Value exception_str(exception);
        LOGE("Exception: %s\n", *exception_str);
    }

    return handleScope.Escape(result.ToLocalChecked());
}

UniquePersistent<Object> V8JSManager::runJSComponent(std::string name, std::string script, SafePtr<ComponentBase> component) {
    HandleScope handleScope(m_isolate);

    auto extendedScript = Tools::format("\n"
                                        "function constructJsComponent(wrapper) {\n"
                                        "   let Component = function () {"
                                        "   }\n"
                                        "\n"
                                        "   Component.prototype = wrapper;\n"
                                        "   Component.prototype.init = ()=>{};\n"
                                        "   Component.prototype.deinit = ()=>{};\n"
                                        "   Component.prototype.update = (dt)=>{};\n"
                                        "\n"
                                        "   %s"
                                        "   \n"
                                        "   let testComponent = new %s();"
                                        "   return testComponent;"
                                        "}", script.c_str(), name.c_str());

    Local <Context> context = Local <Context>::New (m_isolate, m_context);
    Context::Scope contextScope (context);

    runScript(context, extendedScript);
    auto wrapped = wrapComponent(component);

    auto jsObject = callFunction("constructJsComponent", {wrapped});

    return UniquePersistent<Object>(m_isolate, jsObject.As<Object>());
}

void V8JSManager::init() {
    // Initialize V8.
    V8::InitializeICU();
    m_platform = platform::CreateDefaultPlatform();
    V8::InitializePlatform(m_platform);
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    Isolate::CreateParams createParams;
    m_arrayBufferAllocator = new ArrayBufferAllocator;
    createParams.array_buffer_allocator = m_arrayBufferAllocator;
    m_isolate = Isolate::New(createParams);
    m_isolate->Enter();

    HandleScope handleScope(m_isolate);

    Local<ObjectTemplate> global = ObjectTemplate::New(m_isolate);
    global->Set(toV8Str("log"), FunctionTemplate::New(m_isolate, log));

    auto context = Context::New(m_isolate, nullptr, global);

    // Create a new context.
    m_context = UniquePersistent<Context>(m_isolate, context);
}

void V8JSManager::deinit() {
    for (auto pair : m_counterMap) {
        if (pair.second > 0)
            unregisterWrapper(pair.first);
    }
    persistentHolder.clear();
    m_context.Reset();
    m_isolate->Exit();
    m_isolate->Dispose();
    V8::ShutdownPlatform();
    delete m_arrayBufferAllocator;
    delete m_platform;
}

} // flappy
