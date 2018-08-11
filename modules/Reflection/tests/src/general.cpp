#include <iostream>
#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>
#include <tuple>

#include <catch.h>

// Implementation

using TypeId = std::intptr_t;

template <typename T>
TypeId getTypeId() {
    return reinterpret_cast<std::intptr_t>(&getTypeId<T>);
}

class Value {
public:
    template <typename T>
    Value(T&& value)
        : m_value(std::make_shared<T>(std::forward<T>(value)))
        , m_typeId(getTypeId<T>())
    {}
    
    TypeId typeId() const {
        return m_typeId;
    }
    
    void* valuePtr() const {
        return m_value.get();
    }
    
    template <typename T>
    T& as() const {
        if (getTypeId<T>() != m_typeId)
            throw std::runtime_error("Value of wrong type! Expects: " + std::to_string(getTypeId<T>()) + " Passed: " + std::to_string(m_typeId));
        return *static_cast<std::decay_t<T>*>(m_value.get());
    }
    
private:
    std::shared_ptr<void> m_value;
    TypeId m_typeId;
};

class AnyArg {
public:
    AnyArg()
    {}
    
    template <typename T>
    AnyArg(T&& value) 
        : m_valuePtr(&value)
        , m_typeId(getTypeId<T>())
    {}
    
    template <typename T>
    T& as() const {
        if (getTypeId<T>() != m_typeId)
            throw std::runtime_error("Value of wrong type! Expects: " + std::to_string(getTypeId<T>()) + " Passed: " + std::to_string(m_typeId));
        return *static_cast<std::decay_t<T>*>(m_valuePtr);
    }
    
private:
    void* m_valuePtr;
    TypeId m_typeId;
};

template <>
AnyArg::AnyArg (Value& value)
    : m_valuePtr(value.valuePtr())
    , m_typeId(value.typeId())
{}

template<typename ArgsTupleT, typename FuncT,  std::size_t... I>
auto call(FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
    return func(args[I].as<typename std::tuple_element<I, ArgsTupleT>::type>()...);
}

template <typename Result, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
std::function<Value(const std::vector<AnyArg>&)> wrap(Result (* func) (ArgT ...)) {
    using ArgsTuple = std::tuple<ArgT...>;
    return [func] (const std::vector<AnyArg>& anyArgs) -> Value {
        if constexpr (std::is_same<Result, void>::value)
            return call<ArgsTuple>(func, anyArgs,Indices{}), AnyArg();
        else
            return call<ArgsTuple>(func, anyArgs, Indices{});
    };
}

// Test functions

int somePrettyFunction(int a, int b, int& result) {
    std::cout << a << " " << b << std::endl;
    result = a + b;
    return a + b;
}

void test(std::string str) {
    std::cout << str << std::endl;
}

TEST_CASE("General") {
    auto wrappedFunc1 = wrap(&somePrettyFunction);
    int result = 0;
    Value value(10);
    REQUIRE(wrappedFunc1({value, 20, result}).as<int>() == 30);
    REQUIRE(result == 30);
    auto wrappedFunc2 = wrap(&test);
    wrappedFunc2({std::string("Hello, World!")});
}
