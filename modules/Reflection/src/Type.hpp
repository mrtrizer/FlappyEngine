#pragma once

#include <unordered_map>
#include <vector>

#include "TypeId.hpp"
#include "Function.hpp"
#include "ConstructorRef.hpp"
#include "MethodRef.hpp"
#include "Reflection.hpp"

namespace flappy {

class Type {
public:
    // FIXME: Make constructor private
    Type(TypeId typeId, const Reflection& reflection)
        : m_typeId(typeId)
        , m_reflection(reflection)
    {}

    const std::vector<Constructor>& constructors() const { return m_constructors; }

    const std::unordered_map<std::string, Function>& methodsMap() const { return m_methodsMap; }

    const Function& method(std::string name) const { return m_methodsMap.at(name); }

    template <typename ... ArgT>
    Value constructSharedPtr(ArgT&& ... anyArgs) const {
        return findConstructor<ArgT...>(anyArgs...).sharedPtrConstructor(anyArgs...);
    }

    template <typename ... ArgT>
    Value constructUniquePtr(ArgT&& ... anyArgs) const {
        return findConstructor<ArgT...>(anyArgs...).uniquePtrConstructor(anyArgs...);
    }

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

    TypeId typeId() const { return m_typeId; }

    template <typename ReturnT, typename ... ArgT>
    void registerMember(const ConstructorRef<ArgT...>& constructorRef) {
        m_constructors.emplace_back(constructorRef.template generate<ReturnT>(m_reflection));
    }

    template <typename ReturnT, typename ... ArgT>
    void registerMember(const MethodRef<ArgT...>& methodRef) {
        m_methodsMap.emplace(methodRef.name(), methodRef.generate(m_reflection));
    }
private:
    TypeId m_typeId;
    const Reflection& m_reflection;
    std::vector<Constructor> m_constructors;
    std::unordered_map<std::string, Function> m_methodsMap;

    template <typename ... ArgT>
    const Constructor& findConstructor(const ArgT& ... anyArgs) const {
        auto iter = std::find_if(m_constructors.begin(), m_constructors.end(),
                     [&anyArgs...] (const auto& constructor) { return constructor.onStackConstructor.template fitArgs<ArgT...>(anyArgs ...);});
        if (iter != m_constructors.end())
            return *iter;
        throw std::runtime_error("Can't construct");
    }

};

} // flappy
