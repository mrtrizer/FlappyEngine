#pragma once

#include <memory>

#include "TypeId.hpp"
#include "Function.hpp"

namespace flappy {

template <typename ... ArgT>
class ConstructorRef {
public:
    template <typename ResultT>
    Function generate(const Reflection& reflection) const {
        auto lambda = [](ArgT...args) { return ResultT(args...); };
        using Func = ResultT (*) (ArgT...);
        return Function(reflection, static_cast<Func>(lambda) );
    }
};

} // flappy
