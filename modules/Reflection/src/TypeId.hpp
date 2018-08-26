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
    bool isPointer() const { return m_flags[PointerFlagIndex]; }
    bool isConst() const { return m_flags[ConstFlagIndex]; }
    bool canAssignTo(const TypeId& target) const {
        return target.m_serial == m_serial
            && target.isPointer() == isPointer()
            && (target.isConst() || isConst() == target.isConst());
    }
    bool operator==(const TypeId& other) const {
        return m_flags == other.m_flags
                && m_serial == other.m_serial;
    }
    bool operator!=(const TypeId& other) const {
        return !operator==(other);
    }

    bool isValid() const { return m_serial != 0; }

    TypeId(intptr_t serial, bool pointerFlag, bool constFlag)
        : m_serial(serial)
        , m_flags(pointerFlag << PointerFlagIndex | constFlag << ConstFlagIndex)
    {}

    TypeId(TypeId typeId, bool pointerFlag, bool constFlag)
        : TypeId(typeId.m_serial, pointerFlag, constFlag)
    {}

    std::string name;

private:
    static constexpr size_t PointerFlagIndex = 0;
    static constexpr size_t ConstFlagIndex = 1;

    intptr_t m_serial = 0;
    std::bitset<8> m_flags;
};



template <typename T>
intptr_t getTypeSerial() {
    static int placeholder;
    return reinterpret_cast<intptr_t>(&placeholder);
}

template <typename T>
TypeId getTypeId() {
    // FIXME: Remove name parsing
    std::regex regex("Type = ([0-9A-Za-z:<>, ]*)");
    std::smatch match;
    std::string str(__PRETTY_FUNCTION__);
    std::regex_search(str, match, regex);
    TypeId typeId(getTypeSerial<std::remove_pointer_t<std::decay_t<T>>>(),
                  std::is_pointer_v<std::decay_t<T>>,
                  std::is_const_v<std::remove_pointer_t<std::remove_reference_t<T>>>);
    typeId.name = str
            + std::to_string(typeId.isPointer())
            + std::to_string(typeId.isConst());
    return typeId;
}

// FIXME: Remove
std::string getTypeName(TypeId id) {
    return id.name;
}

} // flappy

namespace std {
template <>
struct hash <flappy::TypeId> {
    size_t operator()(const flappy::TypeId& typeId) const noexcept {
        return static_cast<size_t>(typeId.m_serial) * static_cast<size_t>(typeId.m_flags.to_ulong());
    }
};
} // std
