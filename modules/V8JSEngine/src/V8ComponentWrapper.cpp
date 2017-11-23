#include "V8ComponentWrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <Entity.h>
#include <ComponentBase.h>

#include "CppObjectHolder.h"
#include "SafePtrHolder.h"
#include "V8JSUtils.h"
#include "V8EntityWrapper.h"

namespace flappy {

using namespace v8;

static void getType(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    auto component = static_cast<CppObjectHolder<ComponentBase>*>(field->Value())->safePtr();
    auto name = toV8Str(component->componentId().name());
    info.GetReturnValue().Set(name);
}

static void isInitialized(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    auto component = static_cast<CppObjectHolder<ComponentBase>*>(field->Value())->safePtr();
    info.GetReturnValue().Set(component->isInitialized());
}

static void active(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    auto component = static_cast<CppObjectHolder<ComponentBase>*>(field->Value())->safePtr();
    info.GetReturnValue().Set(component->active());
}

static void setActive(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    auto component = static_cast<CppObjectHolder<ComponentBase>*>(field->Value())->safePtr();
    component->setActive(info[0]->BooleanValue());
}

// TODO: Don't wrap entity every time. It would be better to store it somewhere
static void entity(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    auto component = static_cast<CppObjectHolder<ComponentBase>*>(field->Value())->safePtr();
    info.GetReturnValue().Set(wrapEntity(component->entityRef()));
}

Local<Object> wrapComponent(SafePtr<ComponentBase> component) {
    auto isolate = Isolate::GetCurrent();
    EscapableHandleScope handleScope(isolate);
    Local <Context> context = Local <Context>::New (isolate, isolate->GetCurrentContext());
    Context::Scope contextScope (context);

    Local<External> componentJSPtr = External::New(isolate, new SafePtrHolder<ComponentBase>(component));

    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(isolate);
    Local<Template> prototype = funcTemplate->PrototypeTemplate();

    prototype->Set(toV8Str("type"), FunctionTemplate::New(isolate, getType, componentJSPtr));
    prototype->Set(toV8Str("initialized"), FunctionTemplate::New(isolate, isInitialized, componentJSPtr));
    prototype->Set(toV8Str("active"), FunctionTemplate::New(isolate, active, componentJSPtr));
    prototype->Set(toV8Str("setActive"), FunctionTemplate::New(isolate, setActive, componentJSPtr));
    prototype->Set(toV8Str("entity"), FunctionTemplate::New(isolate, entity, componentJSPtr));

    Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, componentTemplate);

    Local<Object> result = templ->NewInstance(context).ToLocalChecked();

    result->SetPrivate(currentContext(), toV8PrivateKey("cpp_ptr"), componentJSPtr);

    return handleScope.Escape(result);
}

} // flappy
