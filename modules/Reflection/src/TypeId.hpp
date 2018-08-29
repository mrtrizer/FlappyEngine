#pragma once

#include <bitset>
#include <type_traits>
#include <regex>
#include <functional>
#include <unordered_map>

#include <Utility.hpp>

namespace flappy {

class TypeId {
    friend class std::hash<TypeId>; // To calculate hash
public:
    TypeId() = default;
    bool isPointer() const { return m_bitset & (intptr_t)1 << PointerFlagIndex; }
    bool isConst() const { return m_bitset & (intptr_t)1 << ConstFlagIndex; }
    bool canAssignTo(const TypeId& target) const {
        return (target.m_bitset & FlagsSet) == (m_bitset & FlagsSet)
            && target.isPointer() == isPointer()
            && (target.isConst() || isConst() == target.isConst());
    }
    bool operator==(const TypeId& other) const {
        return m_bitset == other.m_bitset;
    }
    bool operator!=(const TypeId& other) const {
        return !operator==(other);
    }

    bool isValid() const { return m_bitset != 0; }

    TypeId(intptr_t serial, bool pointerFlag, bool constFlag)
        : m_bitset((serial & ~FlagsSet) | ((intptr_t)pointerFlag << PointerFlagIndex | (intptr_t)constFlag << ConstFlagIndex))
    {}

    TypeId(TypeId typeId, bool pointerFlag, bool constFlag)
        : TypeId(typeId.m_bitset, pointerFlag, constFlag)
    {}

#ifdef DEBUG_TYPE_NAMES
    std::string name;
#endif

private:
    static constexpr size_t PointerFlagIndex = sizeof(intptr_t) * 8 - 1;
    static constexpr size_t ConstFlagIndex = sizeof(intptr_t) * 8 - 2;
    static constexpr intptr_t FlagsSet = (intptr_t)1 << PointerFlagIndex | (intptr_t)1 << ConstFlagIndex;

    intptr_t m_bitset = 0;
};

#ifndef DEBUG_TYPE_NAMES
static_assert(sizeof(TypeId) == sizeof(intptr_t), "Should fit into intptr_t size.");
#endif

template <typename T>
intptr_t getTypeSerial() {
    static int placeholder;
    return reinterpret_cast<intptr_t>(&placeholder);
}

template <typename T>
TypeId getTypeId() {
    TypeId typeId(getTypeSerial<std::remove_pointer_t<std::decay_t<T>>>(),
                  std::is_pointer_v<std::decay_t<T>>,
                  std::is_const_v<std::remove_pointer_t<std::remove_reference_t<T>>>);
#ifdef DEBUG_TYPE_NAMES
    // FIXME: Remove name parsing
    std::regex regex("Type = ([0-9A-Za-z:<>, ]*)");
    std::smatch match;
    std::string str(__PRETTY_FUNCTION__);
    std::regex_search(str, match, regex);
    typeId.name = str
            + std::to_string(typeId.isPointer())
            + std::to_string(typeId.isConst());
#endif
    return typeId;
}

// FIXME: Remove
inline std::string getTypeName(TypeId id) {
#ifdef DEBUG_TYPE_NAMES
    return id.name;
#else
    return "";
#endif
}

inline const static TypeId UnknownType = getTypeId<void>();

} // flappy

namespace std {
template <>
struct hash <flappy::TypeId> {
    size_t operator()(const flappy::TypeId& typeId) const noexcept {
        return static_cast<size_t>(typeId.m_bitset);
    }
};
} // std
