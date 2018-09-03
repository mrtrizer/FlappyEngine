#pragma once

#include "TypeId.hpp"
#include "Function.hpp"
#include "Type.hpp"

namespace flappy {

class Type;

class Reflection : public std::enable_shared_from_this<Reflection> {
public:
    Reflection(const std::shared_ptr<Reflection>& baseReflection = nullptr)
        : m_baseReflection(baseReflection)
    {}

    template <typename TypeT, typename ... ArgT>
    Type& registerType(const std::string& name, const std::vector<TypeId>& parents, const ArgT&... args) {
        auto typePtr = new Type(getTypeId<TypeT>(), parents, *this);
        m_typesMap.emplace(name, std::shared_ptr<Type>(typePtr));
        m_typeNameMap.emplace(getTypeId<TypeT>(), name);
        return *typePtr;
    }

    template <typename TypeT, typename ... ArgT>
    Type& registerType(const std::string& name, const ArgT&... args) {
        return registerType<TypeT>(name, {}, args...);
    }

    bool hasType(const std::string& name) const {
        return m_typesMap.find(name) != m_typesMap.end();
    }

    // FIXME: Check order
    bool hasType(TypeId typeId) const {
        if (m_baseReflection != nullptr)
            return m_baseReflection->hasType(typeId);
        return hasType(m_typeNameMap.at(typeId));
    }

    const Type& getType(const std::string& name) const {
        auto iter = m_typesMap.find(name);
        if (iter != m_typesMap.end())
            return *iter->second;
        if (m_baseReflection != nullptr)
            return m_baseReflection->getType(name);
        throw std::runtime_error(sstr("Type \"", name, "\" is not registered"));
    }

    // FIXME: Check order
    const Type& getType(TypeId typeId) const {
        if (m_baseReflection != nullptr)
            return m_baseReflection->getType(typeId);
        return getType(m_typeNameMap.at(typeId));
    }

    template <typename FunctionT>
    const Function& registerFunction(const std::string& name, FunctionT functionPtr) {
        auto function = std::make_shared<Function>(*this, functionPtr);
        m_functionMap.emplace(name, function);
        return *function;
    }

    bool hasFunction(const std::string& name) const {
        return m_functionMap.find(name) != m_functionMap.end();
    }

    const Function& getFunction(const std::string& name) const {
        auto iter = m_functionMap.find(name);
        if (iter != m_functionMap.end())
            return *iter->second;
        if (m_baseReflection != nullptr)
            return m_baseReflection->getFunction(name);
        throw std::runtime_error(sstr("Function \"", name, "\" is not registered."));
    }

private:
    std::unordered_map<TypeId, std::string> m_typeNameMap;
    // FIXME: Use unique_ptr
    std::unordered_map<std::string, std::shared_ptr<Type>> m_typesMap;
    // FIXME: Use unique_ptr
    std::unordered_map<std::string, std::shared_ptr<Function>> m_functionMap;
    std::shared_ptr<Reflection> m_baseReflection;
};

} // flappy
