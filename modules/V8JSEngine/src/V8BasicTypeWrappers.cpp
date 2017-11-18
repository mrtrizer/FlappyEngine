#include "V8BasicTypeWrappers.h"

#include "V8JSManager.h"

namespace flappy {

void v8DestroyHolder(const v8::WeakCallbackInfo<CppObjectHolderBase> &data) {
     delete data.GetParameter();
}

} // flappy
