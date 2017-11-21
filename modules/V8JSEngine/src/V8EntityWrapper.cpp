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

// We need implement next methods for runtime component access
// Notice that we have to implement virtual name() method for components
// to be able to override it in JSComponent and return name of JS component
// instead of name of a class.
// Actually it's not necessary to add postfix ByName after name of function.
// We could just use overloading.

//void addComponent(std::shared_ptr<ComponentBase> component);

//void removeComponent(std::shared_ptr<ComponentBase> component);

//std::shared_ptr<ComponentBase> findComponentByName(std::string name, unsigned depth = 0);

//std::shared_ptr<ComponentBase> componentByName(std::string name);

//std::list<std::shared_ptr<ComponentBase>> findComponentsByName(std::string name, unsigned depth = 0) const;

//SafePtr<ComponentBase> managerByName(std::string name);

//SafePtr<Entity> parent(); // Ok!

//SafePtr<Entity> root(); // Ok!

//std::shared_ptr<EventController> events() { return m_eventController; } // Ok!

//void addEntity(std::shared_ptr<Entity> entity); // Ok!

//std::shared_ptr<Entity> createEntity(); // Ok!

//void removeEntity(std::shared_ptr<Entity> entity); // Ok!

//std::list<std::shared_ptr<Entity>> findEntities(unsigned depth = 0); // Ok!

// Remove this method. We can use only one method for access to both c++ and js components by name
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
