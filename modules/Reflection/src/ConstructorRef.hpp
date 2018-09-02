#pragma once

#include <memory>

#include "TypeId.hpp"
#include "Function.hpp"

namespace flappy {

struct Constructor {
    Function sharedPtrConstructor;
    Function uniquePtrConstructor;
    Function onStackConstructor;
    Function onHeapConstructor;
    Function inAddressConstructor;
};

template <typename ... ArgT>
class ConstructorRef {
public:
    // FIXME: Support generation of inplace, shared_ptr, and dynamic constructors
    template <typename ResultT>
    Constructor generate(const Reflection& reflection) const {
        auto lambdaSharedPtr = [](ArgT...args) { return std::make_shared<ResultT>(args...); };
        auto lambdaUniquePtr = [](ArgT...args) { return std::make_unique<ResultT>(args...); };
        auto lambdaOnStack = [](ArgT...args) { return ResultT(args...); };
        auto lambdaOnHeap = [](ArgT...args) { return new ResultT(args...); };
        auto lambdaInAdress = [](void* address, ArgT...args) { new(address) ResultT(args...); };
        return Constructor{Function(reflection, static_cast<std::shared_ptr<ResultT> (*) (ArgT...)>(lambdaSharedPtr) ),
                            Function(reflection, static_cast<std::unique_ptr<ResultT> (*) (ArgT...)>(lambdaUniquePtr) ),
                            Function(reflection, static_cast<ResultT (*) (ArgT...)>(lambdaOnStack) ),
                            Function(reflection, static_cast<ResultT* (*) (ArgT...)>(lambdaOnHeap) ),
                            Function(reflection, static_cast<void (*) (void*, ArgT...)>(lambdaInAdress) )};
    }
};

} // flappy
