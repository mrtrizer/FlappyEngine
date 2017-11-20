#include "V8EntityWrapper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <Entity.h>
#include <ComponentBase.h>
#include <JSComponent.h>

#include "V8JSManager.h"

namespace flappy {

using namespace v8;

static void jsComponent(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    void* ptr = field->Value();
    auto entity =  static_cast<Entity*>(ptr);
    String::Utf8Value name(info[0]);
    auto component = entity->findComponent<JSComponent>([&name](const JSComponent& jsComponent) {
        if (jsComponent.name() == *name)
            return true;
        return false;
    });
    info.GetReturnValue().Set(component->jsObject());
}

static void component(const FunctionCallbackInfo<Value>& info) {
    Local<External> field = info.Data().As<External>();
    void* ptr = field->Value();
    auto entity =  static_cast<Entity*>(ptr);
    String::Utf8Value name(info[0]);
    std::string fullName = std::string("flappy::") + *name;
    auto component = entity->componentById(TypeId<ComponentBase>(fullName));
    auto wrapperFunc = wrapperMap.getByName(component->componentId().name()).wrapper;
    auto safePtr = SafePtr<ComponentBase>(component);
    info.GetReturnValue().Set(wrapperFunc(safePtr));
}


Local<Object> wrapEntity(Entity* entity) {

    auto isolate = Isolate::GetCurrent();

    EscapableHandleScope handle_scope(isolate);
    Local <Context> context = Local <Context>::New (isolate, isolate->GetCurrentContext());
    Context::Scope contextScope (context);

    Local<External> jsPtr = External::New(isolate, entity);

    Local<ObjectTemplate> entityTemplate = ObjectTemplate::New(isolate);
    entityTemplate->Set(toV8Str("jsComponent"), FunctionTemplate::New(isolate, jsComponent, jsPtr));
    entityTemplate->Set(toV8Str("component"), FunctionTemplate::New(isolate, component, jsPtr));

    Local<Object> result = entityTemplate->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();

    result->SetPrivate(currentContext(), toV8PrivateKey("cpp_ptr"), jsPtr);

    return handle_scope.Escape(result);
}

} // flappy
