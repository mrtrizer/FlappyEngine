#include <iostream>
#include <cstdlib>
#include <functional>
#include <memory>
#include <vector>
#include <tuple>
#include <regex>
#include <unordered_map>

#include <catch.h>

// Implementation

using TypeId = std::intptr_t;

std::unordered_map<TypeId, std::string> typeNames;

std::string getTypeName(TypeId id) {
    return typeNames[id];
}

template <typename T>
TypeId getTypeId() {
    std::regex regex("Type = ([0-9A-Za-z:<>, ]*)");
    std::smatch match;
    std::string str(__PRETTY_FUNCTION__);
    std::regex_search(str, match, regex);
    typeNames[reinterpret_cast<std::intptr_t>(&getTypeId<T>)] = str;
    return reinterpret_cast<std::intptr_t>(&getTypeId<T>);
}

template <typename... Args>
std::string sstr(Args &&... args) noexcept
{
    std::ostringstream sstr;
    (sstr << ... << args);
    return sstr.str();
}

// FIXME: Value should be defined after AnyArg. But now it's impossible
// FIXME: Value should be deeply copied
class Value {
public:
    Value() = default;
    Value(const Value&) = default;
    Value& operator=(const Value&) = default;
    Value(Value&&) = default;
    Value& operator=(Value&&) = default;

    template <typename T, typename = std::enable_if_t<!std::is_convertible<T, Value>::value>>
    Value(T&& value)
        : m_value(std::make_shared<std::decay_t<T>>(std::forward<T>(value)))
        , m_typeId(getTypeId<std::decay_t<T>>())
    {}

    TypeId typeId() const { return m_typeId; }

    void* voidPointer() const { return m_value.get(); }

    template <typename T>
    std::decay_t<T>& as() const {
        if (getTypeId<std::decay_t<T>>() != m_typeId)
            throw std::runtime_error(sstr("Value of wrong type! Expects: ", getTypeName(getTypeId<std::decay_t<T>>()), " Passed: ", getTypeName(m_typeId)));
        return *static_cast<std::decay_t<T>*>(m_value.get());
    }

private:
    std::shared_ptr<void> m_value;
    TypeId m_typeId = {};
};

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
    std::shared_ptr<Type> registerType(const ArgT&...);

private:
    std::unordered_map<TypeId, std::shared_ptr<Type>> m_typesMap;
};

struct ArgInfo {
    TypeId typeId;
    const bool constFlag = false;
    const bool pointerFlag = false;
    bool operator==(const ArgInfo& argInfo) const {
        return argInfo.typeId == typeId
                && argInfo.pointerFlag == pointerFlag
                && argInfo.constFlag == constFlag;
    }
};

template <typename ArgT>
ArgInfo makeArgInfo() {
    return ArgInfo { getTypeId<std::decay_t<ArgT>>(), std::is_const_v<ArgT>, std::is_pointer_v<ArgT> };
}

class AnyArg {
public:
    AnyArg() = default;
    AnyArg(const AnyArg&) = default;
    AnyArg(AnyArg&&) = default;

    template <typename T >
    AnyArg(T&& value, std::enable_if_t<!std::is_convertible_v<T, Value>>* = 0)
        : m_valuePtr(&value)
        , m_argInfo(makeArgInfo<T>())
    {}

    template <typename T >
    AnyArg (T&& value, std::enable_if_t<std::is_convertible_v<T, Value>>* = 0)
        : m_valuePtr(value.voidPointer())
        , m_argInfo(ArgInfo {value.typeId(), false, false} )
    {}

    template <typename T >
    AnyArg(T* value)
        : m_valuePtr(const_cast<std::decay_t<T>*>(value))
        , m_argInfo(makeArgInfo<T*>())
    {}

    AnyArg(void* valuePtr, TypeId typeId, bool constFlag, bool pointerFlag)
        : m_valuePtr(valuePtr)
        , m_argInfo(ArgInfo {typeId, constFlag, pointerFlag} )
    {}

    // FIXME: Looks like as() functions for pointers and references could be unified
    template <typename T, typename = std::enable_if_t<!std::is_pointer_v<T>> >
    std::decay_t<T>& as(const Reflection& reflection) const {
        if (getTypeId<std::decay_t<T>>() != m_argInfo.typeId) {
            try {
                m_constructedValue = reflection.getType(getTypeId<std::decay_t<T>>())->construct(*this);
                return m_constructedValue.as<std::decay_t<T>>();
            } catch (const std::exception& e) {
                throw std::runtime_error(sstr("No convertion to type ", getTypeName(getTypeId<std::decay_t<T>>())," from type " + getTypeName(m_argInfo.typeId)));
            }
        }
        if (m_argInfo.pointerFlag)
            throw std::runtime_error(sstr("Reference required, but pointer type provided ", getTypeName(m_argInfo.typeId)));
        return *static_cast<std::decay_t<T>*>(m_valuePtr);
    }

    template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    T as(const Reflection& reflection) const {
        if (!m_argInfo.pointerFlag)
            throw std::runtime_error(sstr("Pointer required, but reference type provided ", getTypeName(m_argInfo.typeId)));
        if (m_valuePtr != nullptr && getTypeId<T>() != m_argInfo.typeId)
            throw std::runtime_error(sstr("Wrong pointer type ", getTypeName(getTypeId<T>()), " from type ", getTypeName(m_argInfo.typeId)));
        return static_cast<T>(m_valuePtr);
    }

    const ArgInfo& argInfo() const { return m_argInfo; }
    void* voidPointer() const { return m_valuePtr; }

private:
    void* m_valuePtr = nullptr;
    ArgInfo m_argInfo;
    mutable Value m_constructedValue;
};

// FIXME: Could be implemented as addressof operator in Value class
AnyArg addressOf(const Value& value) {
    return AnyArg(value.voidPointer(), value.typeId(), false, true);
}

AnyArg dereference(const AnyArg& pointer) {
    if (!pointer.argInfo().pointerFlag)
        throw std::runtime_error(sstr("Can't dereference non pointer type ", getTypeName(pointer.argInfo().typeId)));
    return AnyArg(pointer.voidPointer(), pointer.argInfo().typeId, pointer.argInfo().constFlag, false);
}

class Function {
public:
    // Constructor for non-member functions
    template <typename ResultT, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
    Function(const std::shared_ptr<Reflection>& reflection, ResultT (* func) (ArgT ...))
        : m_function([func, reflection] (const std::vector<AnyArg>& anyArgs) -> Value {
            using ArgsTuple = std::tuple<ArgT...>;
            if constexpr (std::is_same<ResultT, void>::value)
                return call<ArgsTuple>(*reflection, func, anyArgs,Indices{}), AnyArg();
            else
                return call<ArgsTuple>(*reflection, func, anyArgs, Indices{});
        })
        , m_argumentTypeIds {makeArgInfo<ArgT>()...}
        , m_classTypeId (0)
        , m_resultTypeId (getTypeId<ResultT>())
    {}

    // Constructor for member functions
    template <typename TypeT, typename ResultT, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
    Function(const std::shared_ptr<Reflection>& reflection, ResultT (TypeT::*func) (ArgT ...))
        : m_function([func, reflection] (const std::vector<AnyArg>& anyArgs) -> Value {
            using ArgsTuple = std::tuple<ArgT...>;
            if constexpr (std::is_same<ResultT, void>::value)
                return callMember<TypeT, ArgsTuple>(*reflection, func, anyArgs, Indices{}), AnyArg();
            else
                return callMember<TypeT, ArgsTuple>(*reflection, func, anyArgs, Indices{});
        })
        , m_argumentTypeIds {makeArgInfo<ArgT>()...}
        , m_classTypeId (getTypeId<TypeT>())
        , m_resultTypeId (getTypeId<ResultT>())
    {}

    template <typename ... ArgT>
    Value operator()(ArgT&& ... anyArgs) const {
        if (sizeof...(ArgT) != m_argumentTypeIds.size() + (m_classTypeId == 0 ? 0 : 1))
            throw std::runtime_error(sstr("Wrong number of arguments. Expected: ", m_argumentTypeIds.size(), " Received: ", sizeof...(ArgT)));
        return m_function(std::vector<AnyArg>{AnyArg(std::forward<ArgT>(anyArgs)) ...});
    }

    template <typename ... ArgT>
    bool fitArgs(const ArgT& ... args) const {
        return sizeof...(ArgT) == m_argumentTypeIds.size() && fitArgsInternal<ArgT...>(0, args ...);
    }

    const std::vector<ArgInfo>& argumentTypeIds() const { return m_argumentTypeIds; }
    TypeId resultTypeId() const { return m_resultTypeId; }

private:
    std::function<Value(const std::vector<AnyArg>& anyArgs)> m_function;
    std::vector<ArgInfo> m_argumentTypeIds;
    TypeId m_classTypeId = 0; // only for member functions
    TypeId m_resultTypeId;

    template<typename ArgsTupleT, typename FuncT,  std::size_t... I>
    static auto call(const Reflection& reflection, FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
        return func(args[I].as<typename std::tuple_element<I, ArgsTupleT>::type>(reflection)...);
    }

    template<typename TypeT, typename ArgsTupleT, typename FuncT,  std::size_t... I>
    static auto callMember(const Reflection& reflection, FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
        return (args.front().as<TypeT&>(reflection).*func)(args[I + 1].as<typename std::tuple_element<I, ArgsTupleT>::type>(reflection)...);
    }

    template <typename...>
    bool fitArgsInternal(size_t) const { return true; }

    template <typename FrontArgT, typename ... ArgT>
    bool fitArgsInternal(size_t index, const FrontArgT& frontArg, const ArgT& ... args) const {
        if constexpr (std::is_same<std::decay_t<FrontArgT>, AnyArg>::value)
            return frontArg.argInfo() == m_argumentTypeIds[index] && fitArgsInternal<ArgT...>(index + 1, args...);
        else
            return makeArgInfo<FrontArgT>() == m_argumentTypeIds[index] && fitArgsInternal<ArgT...>(index + 1, args...);
    }
};

template <typename ... ArgT>
class ConstructorRef {
public:
    template <typename ResultT>
    Function generate(const std::shared_ptr<Reflection>& reflection) const {
        auto lambda = [](ArgT...args) { return ResultT(args...); };
        using Func = ResultT (*) (ArgT...);
        return Function(reflection, static_cast<Func>(lambda) );
    }
};

template <typename TypeT, typename ResultT, typename ... ArgT>
class MethodRef {
public:
    using FunctionPointer =  ResultT (TypeT::*) (ArgT ...);
    using ConstFunctionPointer =  ResultT (TypeT::*) (ArgT ...) const;
    using Lambda = ResultT (*) (TypeT&, ArgT ...);

    explicit MethodRef(std::string name, FunctionPointer method)
        : m_name(name)
        , m_method(method)
    {}

    explicit MethodRef(std::string name, ConstFunctionPointer method)
        : m_name(name)
        , m_method(reinterpret_cast<FunctionPointer>(method))
    {}

    explicit MethodRef(std::string name, Lambda method)
        : m_name(name)
        , m_wrappedMethod(method)
    {}

    Function generate(const std::shared_ptr<Reflection>& reflection) const {
        return m_wrappedMethod ? Function(reflection, m_wrappedMethod) : Function(reflection, m_method);
    }

    const std::string& name() const { return m_name; }

private:
    FunctionPointer m_method;
    Lambda m_wrappedMethod;
    std::string m_name;
};

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

template <typename TypeT, typename ... ArgT>
std::shared_ptr<Type> Reflection::registerType(const ArgT&...args) {
    auto type = std::make_shared<Type>(getTypeId<TypeT>());
    (type->template registerMember<TypeT>(args, shared_from_this()), ...);
    m_typesMap.emplace(type->typeId(), type);
    return type;
}

// Test functions

int somePrettyFunction(int a, int b, int& result) {
    std::cout << a << " " << b << std::endl;
    result = a + b;
    return a + b;
}

void testFunc(std::string str) {
    std::cout << str << std::endl;
}

struct TestClass {
    TestClass(int c)
        : m_c(c)
    {}
    int testMethodConst(int a, int b) const {
        return a * b * m_c;
    }

    int testMethod(int a, int b) {
        return a * b * m_c;
    }

    int m_c = 0;
};

void test() {
    auto reflection = std::make_shared<Reflection>();
    auto stringType = reflection->registerType<std::string>(
                            ConstructorRef<const char*>(),
                            ConstructorRef<std::string>(),
                            ConstructorRef<size_t, char>(),
                            MethodRef<std::string, unsigned long>("capacity", [](std::string& obj){ return obj.capacity(); } )
                );

    auto type = reflection->registerType<TestClass>(
                            MethodRef("testMethod", &TestClass::testMethod),
                            MethodRef("testMethodConst", &TestClass::testMethod)
                );

    TestClass testClass(30);
    auto wrappedFunc1 = Function(reflection, &somePrettyFunction);
    int result = 0;
    std::cout << wrappedFunc1(10, 20, result).as<int>() << std::endl;
    std::cout << result << std::endl;
    std::cout << type->method("testMethod")(testClass, 10, 20).as<int>() << std::endl;

    auto wrappedFunc2 = Function(reflection, &testFunc);
    wrappedFunc2("Hello, World!");
    auto str = reflection->getType(getTypeId<std::string>())->construct(size_t(10), 'a');
    auto& strRef = str;
    wrappedFunc2(str);
    std::cout << reflection->getType(getTypeId<std::string>())->method("capacity")(strRef).as<unsigned long>() << std::endl;
}

TEST_CASE("General") {
    test();
}
