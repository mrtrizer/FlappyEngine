#pragma once

#include <Manager.h>

#include <v8.h>

namespace flappy {

v8::Local<v8::Object> wrapEntity(SafePtr<Entity> entity);

} // flappy
