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
        static std::true_type Test(decltype(std::declval<C>().template signature)*); \
        \
        template<typename> \
        static std::false_type& Test(...); \
        \
    public: \
        static bool const value = std::is_same_v<decltype(Test<T>(0)), std::true_type>; \
    };

CHECK_MEMBER_FUNC(HasCopyConstructor, T(std::declval<T>()))
CHECK_MEMBER_FUNC(HasCopyAssignOperator, operator=(std::declval<T>()))
CHECK_MEMBER_FUNC(HasMoveConstructor, T(std::move(std::declval<T>())))
CHECK_MEMBER_FUNC(HasMoveAssignOperator, operator=(std::move(std::declval<T>())))

class Value : public ValueRef {
private:
    void* m_object = nullptr;
    std::function<void(void*)> m_deleteObject;
    std::function<void*(const void*)> m_copyObject;
    std::function<void*(void*, const void*)> m_copyAssignObject;
    std::function<void*(const void*)> m_moveObject;
    std::function<void*(void*, const void*)> m_moveAssignObject;
public:
    Value() = default;
    Value(const Value& other) {
        if (other.m_copyObject == nullptr)
            throw FlappyException("Value doesn't have a copy constructor");
        m_object = other.m_copyObject(other.m_object);
    }
    Value& operator=(const Value& other) {
        if (other.m_copyAssignObject == nullptr)
            throw FlappyException("Value doesn't have a copy assignment operator");
        other.m_copyAssignObject(m_object, other.m_object);
        return *this;
    }
    Value(Value&& other) {
        if (other.m_moveObject == nullptr)
            throw FlappyException("Value doesn't have a move constructor");
        m_object = other.m_moveObject(other.m_object);
    }
    Value& operator=(Value&& other) {
        if (other.m_moveAssignObject == nullptr)
            throw FlappyException("Value doesn't have a move assignment operator");
        other.m_moveAssignObject(m_object, other.m_object);
        return *this;
    }
    ~Value() {
        if (m_object != nullptr)
            m_deleteObject(m_object);
    }
    
    template <typename T, typename = std::enable_if_t<!std::is_convertible<T, Value>::value>>
    Value(T* value, TypeId typeId)
        : m_object(value)
        , m_deleteObject([](void* obj) { delete static_cast<T*>(obj); } )
    {
        if constexpr (HasCopyConstructor<T>::value)
            m_copyObject = [](void* obj) -> void* { return new T(*static_cast<T*>(obj)); };
        if constexpr (HasCopyAssignOperator<T>::value)
            m_copyAssignObject = [](void* to, void* from){ return *static_cast<T*>(to) = *static_cast<T*>(from); };
        setValuePtr(m_object, typeId);
    }

    template <typename T, typename = std::enable_if_t<!std::is_convertible<T, Value>::value>>
    Value(T&& value)
        : m_object(new std::decay_t<T>(std::forward<T>(value)))
        , m_deleteObject([](void* obj) { delete static_cast<T*>(obj); } )
    {
        if constexpr (HasCopyConstructor<T>::value)
            m_copyObject = [](void* obj) -> void* { return new T(*static_cast<T*>(obj)); };
        if constexpr (HasCopyAssignOperator<T>::value)
            m_copyAssignObject = [](void* to, void* from){ return *static_cast<T*>(to) = *static_cast<T*>(from); };
        setValuePtr(m_object, getTypeId<T>());
    }

    ValueRef deref() {
        if (!typeId().isPointer())
            throw FlappyException("Type is not a pointer");
        return ValueRef(m_object, TypeId(typeId(), false, typeId().isConst()));
    }


};

} // flappy
