#include "V8JSUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>

#include <libplatform/libplatform.h>
#include <v8.h>

namespace flappy {

using namespace v8;

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

std::vector<v8::UniquePersistent<v8::External>> persistentHolder;
TypeMap<void, Wrapper> wrapperMap;

} // flappy
