#pragma once

#include <memory>

#include "TypeId.hpp"
#include "Function.hpp"
#include "Constructor.hpp"

namespace flappy {

template <typename ... ArgT>
class ConstructorRef {
public:
    // FIXME: Support generation of inplace, shared_ptr, and dynamic constructors
    template <typename ResultT>
    Constructor generate(const Reflection& reflection) const {
        auto lambdaOnStack = [](ArgT...args) { return ResultT(args...); };
        auto lambdaOnHeap = [](ArgT...args) { return new ResultT(args...); };
        auto lambdaInAdress = [](void* address, ArgT...args) { new(address) ResultT(args...); };
        return Constructor{ Function(reflection, static_cast<ResultT (*) (ArgT...)>(lambdaOnStack) ),
                            Function(reflection, static_cast<ResultT* (*) (ArgT...)>(lambdaOnHeap) ),
                            Function(reflection, static_cast<void (*) (void*, ArgT...)>(lambdaInAdress) )};
    }
};

} // flappy
