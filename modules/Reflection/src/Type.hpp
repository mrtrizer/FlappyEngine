#pragma once

#include <unordered_map>
#include <vector>

#include "TypeId.hpp"
#include "Function.hpp"
#include "Constructor.hpp"
#include "Field.hpp"

namespace flappy {

// FIXME: Rename "method" to "function" (as in C++ specification)
// TODO: Introduce TypeBuilder to get rid of passing type to some member registration functions
class Type {
    friend class Reflection; // To construct
public:
    TypeId typeId() const { return m_typeId; }

    const std::vector<TypeId>& parents() { return m_parents; }

    const std::vector<Constructor>& constructors() const { return m_constructors; }

    const std::unordered_map<std::string, Function>& functionMap() const { return m_functionsMap; }

    const Function& function(const std::string& name) const { return m_functionsMap.at(name); }

    template <typename ... ArgT>
    Value constructOnStack(ArgT&& ... anyArgs) const {
        return findConstructor<ArgT...>(anyArgs...).onStackConstructor(anyArgs...);
    }

    template <typename ... ArgT>
    Value constructOnHeap(ArgT&& ... anyArgs) const {
        return findConstructor<ArgT...>(anyArgs...).onHeapConstructor(anyArgs...);
    }

    template <typename ... ArgT>
    void constructInAddress(void* address, ArgT&& ... anyArgs) const {
         findConstructor<ArgT...>(anyArgs...).inAddressConstructor(address, anyArgs...);
    }

    template <typename ResultT, typename ... ArgT>
    Type& addConstructor() {
        m_constructors.emplace_back(Constructor {
            Function(m_reflection, static_cast<Value (*) (ArgT...)>([](ArgT...args) { return Value(new ResultT(args...), getTypeId<ResultT>()); }) ),
            Function(m_reflection, static_cast<ResultT* (*) (ArgT...)>([](ArgT...args) { return new ResultT(args...); }) ),
            Function(m_reflection, static_cast<void (*) (void*, ArgT...)>([](void* ptr, ArgT...args) { new(ptr) ResultT(args...); }) )});
        return *this;
    }

    template <typename FuncT>
    Type& addFunction(const std::string& name, FuncT func) {
        m_functionsMap.emplace(name, Function(m_reflection, func) );
        return *this;
    }

    template <typename TypeT, typename ResultT, typename ... ArgT, typename FuncT>
    Type& addFunction(const std::string& name, FuncT lambda) {
        m_functionsMap.emplace(name, Function(m_reflection, std::function<ResultT(TypeT&, ArgT...)>(lambda)));
        return *this;
    }

    template <typename TypeT, typename FieldT>
    Type& addField(const std::string& name, FieldT TypeT::*fieldPtr) {
        m_fieldsMap.emplace(name, Field(m_reflection, fieldPtr));
        return *this;
    }

    const Field& field(const std::string& name) const { return m_fieldsMap.at(name); }

private:
    TypeId m_typeId;
    std::vector<TypeId> m_parents;
    const Reflection& m_reflection;
    std::vector<Constructor> m_constructors;
    std::unordered_map<std::string, Function> m_functionsMap;
    std::unordered_map<std::string, Field> m_fieldsMap;

    Type(TypeId typeId, const std::vector<TypeId>& parents, const Reflection& reflection)
        : m_typeId(typeId)
        , m_parents(parents)
        , m_reflection(reflection)
    {}

    template <typename ... ArgT>
    const Constructor& findConstructor(const ArgT& ... anyArgs) const {
        auto iter = std::find_if(m_constructors.begin(), m_constructors.end(),
                     [&anyArgs...] (const auto& constructor) { return constructor.onStackConstructor.template fitArgs<ArgT...>(anyArgs ...);});
        if (iter != m_constructors.end())
            return *iter;
        throw FlappyException("Can't construct");
    }
};

} // flappy
