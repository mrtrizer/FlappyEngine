#pragma once

#include <unordered_map>
#include <vector>

#include "TypeId.hpp"
#include "Function.hpp"
#include "ConstructorRef.hpp"
#include "MethodRef.hpp"

namespace flappy {

class Type {
public:
    Type(TypeId typeId)
        : m_typeId(typeId)
    {}

    const std::vector<Function>& constructors() const { return m_constructors; }

    const std::unordered_map<std::string, Function>& methodsMap() const { return m_methodsMap; }

    const Function& method(std::string name) const { return m_methodsMap.at(name); }

    template <typename ... ArgT>
    Value construct(ArgT&& ... anyArgs) const {
        for (const auto& constructor : m_constructors)
            if (constructor.fitArgs<ArgT...>(anyArgs ...))
                return constructor(std::forward<ArgT>(anyArgs)...);
        throw std::runtime_error("Can't construct");
    }

    TypeId typeId() const { return m_typeId; }

    template <typename ReturnT, typename ... ArgT>
    void registerMember(const ConstructorRef<ArgT...>& constructorRef, const std::shared_ptr<Reflection>& reflection) {
        m_constructors.emplace_back(constructorRef.template generate<ReturnT>(reflection));
    }

    template <typename ReturnT, typename ... ArgT>
    void registerMember(const MethodRef<ArgT...>& methodRef, const std::shared_ptr<Reflection>& reflection) {
        m_methodsMap.emplace(methodRef.name(), methodRef.generate(reflection));
    }
private:
    TypeId m_typeId;
    std::vector<Function> m_constructors;
    std::unordered_map<std::string, Function> m_methodsMap;
};

} // flappy
