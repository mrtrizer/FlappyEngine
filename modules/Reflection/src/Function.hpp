#pragma once

#include <tuple>
#include <functional>

#include "Value.hpp"
#include "AnyArg.hpp"

namespace flappy {

class Reflection;

class Function {
public:
    // FIXME: Make constructors private
    // Constructor for non-member functions
    template <typename ResultT, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
    Function(const Reflection& reflection, ResultT (* func) (ArgT ...))
        : m_function([func, &reflection] (const std::vector<AnyArg>& anyArgs) -> Value {
            using ArgsTuple = std::tuple<ArgT...>;
            if constexpr (std::is_same<ResultT, void>::value)
                return call<ArgsTuple>(reflection, func, anyArgs, Indices{}), Value::makeVoid();
            else
                return call<ArgsTuple>(reflection, func, anyArgs, Indices{});
        })
        , m_argumentTypeIds {getTypeId<ArgT>()...}
        , m_resultTypeId (getTypeId<ResultT>())
    {}

    // Constructor for purely inline member functions (usually a lambda taking object refference as an argument)
    // Typically inline member function in standard library don't have an address at all, that's why this hack is needed.
    template <typename TypeT, typename ResultT, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
    Function(const Reflection& reflection, const std::function<ResultT(TypeT&, ArgT ...)>& func)
        : m_function([func, &reflection] (const std::vector<AnyArg>& anyArgs) -> Value {
            using ArgsTuple = std::tuple<ArgT...>;
            if constexpr (std::is_same<ResultT, void>::value)
                return callInlineMember<TypeT, ArgsTuple>(reflection, func, anyArgs, Indices{}), Value::makeVoid();
            else
                return callInlineMember<TypeT, ArgsTuple>(reflection, func, anyArgs, Indices{});
        })
        , m_argumentTypeIds {getTypeId<ArgT>()...}
        , m_classTypeId (getTypeId<TypeT>())
        , m_resultTypeId (getTypeId<ResultT>())
    {}

    // Constructor for member functions
    template <typename TypeT, typename ResultT, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
    Function(const Reflection& reflection, ResultT (TypeT::*func) (ArgT ...))
        : m_function([func, &reflection] (const std::vector<AnyArg>& anyArgs) -> Value {
            using ArgsTuple = std::tuple<ArgT...>;
            if constexpr (std::is_same<ResultT, void>::value)
                return callMember<TypeT, ArgsTuple>(reflection, func, anyArgs, Indices{}), Value::makeVoid();
            else
                return callMember<TypeT, ArgsTuple>(reflection, func, anyArgs, Indices{});
        })
        , m_argumentTypeIds {getTypeId<ArgT>()...}
        , m_classTypeId (getTypeId<TypeT>())
        , m_resultTypeId (getTypeId<ResultT>())
    {}

    // Constructor for const member functions
    template <typename TypeT, typename ResultT, typename ... ArgT>
    Function(const Reflection& reflection, ResultT (TypeT::*func) (ArgT ...) const)
        : Function(reflection, reinterpret_cast<ResultT (TypeT::*) (ArgT ...)>(func))
    {}

    template <typename ... ArgT>
    Value operator()(ArgT&& ... anyArgs) const {
        auto totalArgsNum = m_argumentTypeIds.size() + (m_classTypeId.isValid() ? 1 : 0);
        if (sizeof...(ArgT) != totalArgsNum)
            throw std::runtime_error(sstr("Wrong number of arguments. Expected: ", totalArgsNum, " Received: ", sizeof...(ArgT)));
        return m_function(std::vector<AnyArg>{ AnyArg(std::forward<ArgT>(anyArgs)) ...});
    }

    template <typename ... ArgT>
    bool fitArgs(const ArgT& ... args) const {
        return sizeof...(ArgT) == m_argumentTypeIds.size() && fitArgsInternal(0, args ...);
    }

    const std::vector<TypeId>& argumentTypeIds() const { return m_argumentTypeIds; }
    TypeId resultTypeId() const { return m_resultTypeId; }

private:
    std::function<Value(const std::vector<AnyArg>& anyArgs)> m_function;
    std::vector<TypeId> m_argumentTypeIds;
    TypeId m_classTypeId; // only for member functions
    TypeId m_resultTypeId;

    template<typename ArgsTupleT, typename FuncT,  std::size_t... I>
    static auto call(const Reflection& reflection, FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
        return func(args[I].as<typename std::tuple_element<I, ArgsTupleT>::type>(reflection)...);
    }

    template<typename TypeT, typename ArgsTupleT, typename FuncT,  std::size_t... I>
    static auto callMember(const Reflection& reflection, FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
        return (args.front().as<TypeT&>(reflection).*func)(args[I + 1].as<typename std::tuple_element<I, ArgsTupleT>::type>(reflection)...);
    }

    template<typename TypeT, typename ArgsTupleT, typename FuncT,  std::size_t... I>
    static auto callInlineMember(const Reflection& reflection, FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
        return func(args.front().as<TypeT&>(reflection), args[I + 1].as<typename std::tuple_element<I, ArgsTupleT>::type>(reflection)...);
    }

    template <typename...>
    bool fitArgsInternal(size_t) const { return true; }

    template <typename FrontArgT, typename ... ArgT>
    bool fitArgsInternal(size_t index, const FrontArgT& frontArg, const ArgT& ... args) const {
        if constexpr (std::is_same_v<std::decay_t<FrontArgT>, AnyArg>)
            return frontArg.valueRef().typeId().canAssignTo(m_argumentTypeIds[index]) && fitArgsInternal<ArgT...>(index + 1, args...);
        else if constexpr (std::is_convertible_v<std::decay_t<FrontArgT>&, ValueRef&>)
            return frontArg.typeId().canAssignTo(m_argumentTypeIds[index]) && fitArgsInternal<ArgT...>(index + 1, args...);
        else
            return getTypeId<FrontArgT>().canAssignTo(m_argumentTypeIds[index]) && fitArgsInternal<ArgT...>(index + 1, args...);
    }
};

} // flappy
