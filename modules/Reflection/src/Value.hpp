#pragma once

#include <memory>

#include "ValueRef.hpp"
#include "TypeId.hpp"

namespace flappy {

#define CHECK_MEMBER_FUNC(name, signature) \
    template <typename T> \
    class name { \
    private: \
        template<typename C> \
        static std::true_type Test(std::decay_t<decltype( (signature) )>*); \
        \
        template<typename> \
        static std::false_type& Test(...); \
        \
    public: \
        static bool const value = std::is_same_v<decltype(Test<T>(0)), std::true_type>; \
    };

template<typename T, typename U, typename = void>
struct is_assignable: std::false_type {};

template<typename T, typename U>
struct is_assignable<T, U, decltype(std::declval<T>() = std::declval<U>(), void())>: std::true_type {};


CHECK_MEMBER_FUNC(HasCopyConstructor, C(std::declval<C>()))
CHECK_MEMBER_FUNC(HasCopyAssignOperator, (std::declval<C>() = std::declval<C>(), void() ) )
CHECK_MEMBER_FUNC(HasMoveConstructor, C(std::move(std::declval<C>())))
CHECK_MEMBER_FUNC(HasMoveAssignOperator, std::declval<C>() = std::move(std::declval<C>()))

class Value : public ValueRef {
private:
    std::function<void(void*)> m_deleteObject;
    std::function<void*(const void*)> m_copyObject;
    std::function<void(void*, const void*)> m_copyAssignObject;
    std::function<void*(const void*)> m_moveObject;
    std::function<void(void*, const void*)> m_moveAssignObject;
public:
    void* callCopyConstructor(const Value& other) {
        if (other.voidPointer() == nullptr)
            throw FlappyException("Source value is not initialized");
        if (other.m_copyObject == nullptr)
            throw FlappyException("Value doesn't have a copy constructor");
        return other.m_copyObject(other.voidPointer());
    }

    Value(const Value& other) : ValueRef(callCopyConstructor(other), other.typeId()) {
        copyConstructors(other);
    }
    
    Value& operator=(const Value& other) {
        if (other.voidPointer() == nullptr)
            throw FlappyException("Source value is not initialized");
        if (other.typeId() != typeId())
            throw FlappyException("Assignment of values of different types is not supported yet");
        if (other.m_copyAssignObject == nullptr)
            throw FlappyException("Value doesn't have a copy assignment operator");
        if (voidPointer() == nullptr)
            throw FlappyException("Invalid value");
        other.m_copyAssignObject(voidPointer(), other.voidPointer());
        return *this;
    }
    
    void* callMoveConstructor(const Value& other) {
        if (other.voidPointer() == nullptr)
            throw FlappyException("Source value is not initialized");
        if (other.m_moveObject == nullptr)
            throw FlappyException("Value doesn't have a move constructor");
        return other.m_moveObject(other.voidPointer());
    }
    
    Value(Value&& other) : ValueRef(callMoveConstructor(other), other.typeId()) {
        copyConstructors(other);
    }
    
    Value& operator=(Value&& other) {
        if (other.voidPointer() == nullptr)
            throw FlappyException("Source value is not initialized");
        if (other.typeId() != typeId())
            throw FlappyException("Assignment of values of different types is not supported yet");
        if (other.m_moveAssignObject == nullptr)
            throw FlappyException("Value doesn't have a move assignment operator");
        if (voidPointer() == nullptr)
            throw FlappyException("Invalid value");
        other.m_moveAssignObject(voidPointer(), other.voidPointer());
        return *this;
    }
    
    ~Value() {
        if (voidPointer() != nullptr)
            m_deleteObject(voidPointer());
    }
    
    template <typename T, typename = std::enable_if_t<!std::is_convertible<T, Value>::value>>
    Value(T* value, TypeId typeId)
        : ValueRef(value, typeId)
    {
        if constexpr (! std::is_same_v<void, T>) {
            m_deleteObject = [](void* obj) { delete static_cast<T*>(obj); };
            if constexpr (HasCopyConstructor<T>::value)
                m_copyObject = [](const void* obj) -> void* { return new T(*static_cast<const T*>(obj)); };
            if constexpr (HasCopyAssignOperator<T>::value)
                m_copyAssignObject = [](void* to, const void* from){ *static_cast<T*>(to) = *static_cast<const T*>(from); };
            if constexpr (HasMoveConstructor<T>::value)
                m_moveObject = [](const void* obj) -> void* { return new T(std::move(*static_cast<const T*>(obj))); };
            if constexpr (HasMoveAssignOperator<T>::value)
                m_moveAssignObject = [](void* to, const void* from){ *static_cast<T*>(to) = std::move(*static_cast<const T*>(from)); };
        }
    }

    template <typename T,typename = std::enable_if_t<!std::is_convertible<T, Value>::value>>
    Value(T&& value)
        : Value(new std::decay_t<T>(std::forward<T>(value)), getTypeId<std::decay_t<T>>())
    {
    }

    ValueRef deref() {
        if (!typeId().isPointer())
            throw FlappyException("Type is not a pointer");
        return ValueRef(*static_cast<void**>(voidPointer()), TypeId(typeId(), false, typeId().isConst()));
    }
    
    Value addressOf() {
        if (typeId().isPointer())
            throw FlappyException("Address of pointer is not supported yet!");
        return Value(new void*(const_cast<void*>(voidPointer())), TypeId(typeId(), true, typeId().isConst()));
    }

    static Value makeVoid() { return Value{static_cast<void*>(nullptr), getTypeId<void>()}; }

private:
    void copyConstructors(const Value& other) {
        m_deleteObject = other.m_deleteObject;
        m_copyObject = other.m_copyObject;
        m_copyAssignObject = other.m_copyAssignObject;
        m_moveObject = other.m_moveObject;
        m_moveAssignObject = other.m_moveAssignObject;
    }
};

} // flappy
