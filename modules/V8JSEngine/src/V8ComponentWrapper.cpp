#include "V8ComponentWrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <Entity.h>
#include <ComponentBase.h>
#include <JSComponent.h>

#include "V8EntityWrapper.h"

namespace flappy {

using namespace v8;

template <typename ObjT>
static ComponentBase* unwrapComponent(Local<Object> object) {
    auto internalValue = object->GetPrivate(currentContext(), toV8PrivateKey("cpp_ptr")).ToLocalChecked();
    v8::Local<v8::External> internal = internalValue.As<v8::External>();
    void* ptr = internal->Value();
    return static_cast<ObjT*>(ptr);
}

static void getType(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    auto path = toV8Str(component->componentId().name());
    info.GetReturnValue().Set(path);
}

static void isInitialized(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    info.GetReturnValue().Set(component->isInitialized());
}

static void active(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    info.GetReturnValue().Set(component->active());
}

static void setActive(Local<String>, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    component->setActive(value->BooleanValue());
}

static void testFunc(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    void* ptr = field->Value();
    auto component =  static_cast<ComponentBase*>(ptr);
    auto name = toV8Str(component->componentId().name());
    info.GetReturnValue().Set(name);
}

// TODO: Don't wrap entity every time. It would be better to keep it somewhere else
static void entity(Local<String>, const PropertyCallbackInfo<Value>& info) {
    ComponentBase* component = unwrapComponent<ComponentBase>(info.Holder());
    info.GetReturnValue().Set(wrapEntity(component->entity()));
}

Local<Object> wrapComponent(ComponentBase* component) {
    auto isolate = Isolate::GetCurrent();
    EscapableHandleScope handleScope(isolate);
    Local <Context> context = Local <Context>::New (isolate, isolate->GetCurrentContext());
    Context::Scope contextScope (context);

    Local<External> componentJSPtr = External::New(isolate, component);

    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(isolate);
    Local<Template> prototype = funcTemplate->PrototypeTemplate();
    prototype->Set(toV8Str("testFunc"), FunctionTemplate::New(isolate, testFunc, componentJSPtr));
    Local<ObjectTemplate> componentTemplate = funcTemplate->InstanceTemplate();

    // Add accessors for each of the fields of the request.
    componentTemplate->SetAccessor(toV8Str("type"), getType);
    componentTemplate->SetAccessor(toV8Str("initialized"), isInitialized);
    componentTemplate->SetAccessor(toV8Str("active"), active, setActive);
    componentTemplate->SetAccessor(toV8Str("entity"), entity);

    Local<ObjectTemplate> templ = Local<ObjectTemplate>::New(isolate, componentTemplate);

    Local<Object> result = templ->NewInstance(context).ToLocalChecked();

    result->SetPrivate(currentContext(), toV8PrivateKey("cpp_ptr"), componentJSPtr);

    return handleScope.Escape(result);
}

} // flappy
