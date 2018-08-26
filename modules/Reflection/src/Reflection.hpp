#pragma once

#include "TypeId.hpp"

namespace flappy {

class Type;

class Reflection : public std::enable_shared_from_this<Reflection> {
public:
    const std::shared_ptr<Type>& getType(TypeId typeId) const {
        auto iter = m_typesMap.find(typeId);
        if (iter != m_typesMap.end())
            return iter->second;
        throw std::runtime_error(sstr("Type is not registered. Requested: ", getTypeName(typeId)));
    }

    template <typename TypeT, typename ... ArgT>
    std::shared_ptr<Type> registerType(const ArgT&... args) {
        auto type = std::make_shared<Type>(getTypeId<TypeT>());
        [this, args...] (const auto& type) {
            (type->template registerMember<TypeT>(args, shared_from_this()), ...);
        } (type);

        m_typesMap.emplace(type->typeId(), type);
        return type;
    }

private:
    std::unordered_map<TypeId, std::shared_ptr<Type>> m_typesMap;

    std::shared_ptr<Type> createType(const TypeId& typeId);
};

} // flappy
