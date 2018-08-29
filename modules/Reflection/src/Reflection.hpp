#pragma once

#include "TypeId.hpp"
#include "Function.hpp"
#include "Type.hpp"

namespace flappy {

class Type;

class Reflection : public std::enable_shared_from_this<Reflection> {
public:
    template <typename TypeT, typename ... ArgT>
    const Type& registerType(const ArgT&... args) {
        auto type = std::make_shared<Type>(getTypeId<TypeT>(), *this);
        (type->template registerMember<TypeT>(args), ...);
        m_typesMap.emplace(type->typeId(), type);
        return *type;
    }

    bool hasType(const TypeId& typeId) {
        return m_typesMap.find(typeId) != m_typesMap.end();
    }

    // FIXME: Do something with this. Register type by name maybe? Or introduce function to search TypeId by name.
    const Type& getType(const TypeId& typeId) const {
        auto iter = m_typesMap.find(typeId);
        if (iter != m_typesMap.end())
            return *iter->second;
        throw std::runtime_error(sstr("Type is not registered. Requested: ", getTypeName(typeId)));
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
        throw std::runtime_error(sstr("Type is not registered. Requested: ", name));
    }

private:
    std::unordered_map<TypeId, std::shared_ptr<Type>> m_typesMap;
    std::unordered_map<std::string, std::shared_ptr<Function>> m_functionMap;

    std::shared_ptr<Type> createType(const TypeId& typeId);
};

} // flappy
