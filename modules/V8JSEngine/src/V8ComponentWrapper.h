#pragma once

#include <Manager.h>

#include <v8.h>

namespace flappy {

v8::Local<v8::Object> wrapComponent(SafePtr<ComponentBase> component);

} // flappy
