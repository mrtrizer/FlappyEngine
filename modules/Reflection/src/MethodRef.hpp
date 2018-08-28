#pragma once

#include <string>

#include "Function.hpp"

namespace flappy {

template <typename TypeT, typename ResultT, typename ... ArgT>
class MethodRef {
public:
    using FunctionPointer =  ResultT (TypeT::*) (ArgT ...);
    using ConstFunctionPointer =  ResultT (TypeT::*) (ArgT ...) const;
    using Lambda = ResultT (*) (TypeT&, ArgT ...);

    explicit MethodRef(std::string name, FunctionPointer method)
        : m_name(name)
        , m_method(method)
    {}

    explicit MethodRef(std::string name, ConstFunctionPointer method)
        : m_name(name)
        , m_method(reinterpret_cast<FunctionPointer>(method))
    {}

    explicit MethodRef(std::string name, Lambda method)
        : m_name(name)
        , m_wrappedMethod(method)
    {}

    Function generate(const Reflection& reflection) const {
        return m_wrappedMethod ? Function(reflection, m_wrappedMethod) : Function(reflection, m_method);
    }

    const std::string& name() const { return m_name; }

private:
    FunctionPointer m_method = nullptr;
    Lambda m_wrappedMethod = nullptr;
    std::string m_name;
};

} // flappy
