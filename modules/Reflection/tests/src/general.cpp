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
        , m_typeId(getTypeId<T>())
    {}

    TypeId typeId() const { return m_typeId; }

    void* valuePtr() const { return m_value.get(); }

    template <typename T>
    T& as() const {
        if (getTypeId<T>() != m_typeId)
            throw std::runtime_error("Value of wrong type! Expects: " + std::to_string(getTypeId<T>()) + " Passed: " + std::to_string(m_typeId));
        return *static_cast<std::decay_t<T>*>(m_value.get());
    }

private:
    std::shared_ptr<void> m_value;
    TypeId m_typeId = {};
};

class Type;

class Reflection : public std::enable_shared_from_this<Reflection> {
public:
    void addType(const std::shared_ptr<Type>& type);

    const std::shared_ptr<Type>& getType(TypeId typeId) const {
        auto iter = m_types.find(typeId);
        if (iter != m_types.end())
            return iter->second;
        throw std::runtime_error("Type is not registered. Requested: " + getTypeName(typeId));
    }

    template <typename TypeT, typename ... ArgT>
    void registerType(const ArgT&...);

private:
    std::unordered_map<TypeId, std::shared_ptr<Type>> m_types;
};

class AnyArg {
public:
    AnyArg() = default;
    AnyArg(const AnyArg&) = default;
    AnyArg(AnyArg&&) = default;

    template <typename T, typename = std::enable_if_t<!std::is_pointer_v<T>> >
    AnyArg(T&& value)
        : m_valuePtr(&value)
        , m_typeId(getTypeId<T>())
    {}

    template <typename T>
    AnyArg(T* value)
        : m_valuePtr(const_cast<std::decay_t<T>*>(value))
        , m_typeId(getTypeId<T*>())
    {}

    AnyArg(void* valuePtr, TypeId typeId)
        : m_valuePtr(valuePtr)
        , m_typeId(typeId)
    {}

    template <typename T, typename = std::enable_if_t<!std::is_pointer_v<T>> >
    T& as(const Reflection& reflection) const {
        if (getTypeId<T>() != m_typeId) {
            try {
                m_constructedValue = reflection.getType(getTypeId<T>())->construct(*this);
                return m_constructedValue.as<T>();
            } catch (const std::exception& e) {
                throw std::runtime_error("No convertion to type " + getTypeName(getTypeId<T>()) + " from type " + getTypeName(m_typeId));
            }
        }
        return *static_cast<std::decay_t<T>*>(m_valuePtr);
    }

    template <typename T, typename = std::enable_if_t<std::is_pointer_v<T>>>
    T as(const Reflection& reflection) const {
        if (getTypeId<T>() != m_typeId) {
            throw std::runtime_error("Wrong pointer type " + getTypeName(getTypeId<T>()) + " from type " + getTypeName(m_typeId));
        }
        return static_cast<T>(m_valuePtr);
    }

    TypeId typeId() const { return m_typeId; }

private:
    void* m_valuePtr;
    TypeId m_typeId;
    mutable Value m_constructedValue;
};

template <>
AnyArg::AnyArg (const Value& value)
    : m_valuePtr(value.valuePtr())
    , m_typeId(value.typeId())
{}

template <>
AnyArg::AnyArg (Value&& value)
    : m_valuePtr(value.valuePtr())
    , m_typeId(value.typeId())
{}

class Function {
public:
    template <typename ResultT, typename ... ArgT, typename Indices = std::make_index_sequence<sizeof...(ArgT)>>
    Function(const std::shared_ptr<Reflection>& reflection, ResultT (* func) (ArgT ...))
        : m_function([func, reflection] (const std::vector<AnyArg>& anyArgs) -> Value {
            using ArgsTuple = std::tuple<ArgT...>;
            if constexpr (std::is_same<ResultT, void>::value)
                return call<ArgsTuple>(*reflection, func, anyArgs,Indices{}), AnyArg();
            else
                return call<ArgsTuple>(*reflection, func, anyArgs, Indices{});
        })
        , m_argumentTypeIds {getTypeId<ArgT>()...}
        , m_resultTypeId (getTypeId<ResultT>())
    {}

    template <typename ... ArgT>
    Value operator()(ArgT&& ... anyArgs) const {
        if (sizeof...(ArgT) != m_argumentTypeIds.size())
            throw std::runtime_error("Wrong number of arguments. Expected: " + std::to_string(m_argumentTypeIds.size()) + " Received: " + std::to_string(sizeof...(ArgT)));
        return m_function(std::vector<AnyArg>{AnyArg(std::forward<ArgT>(anyArgs)) ...});
    }

    template <typename ... ArgT>
    bool fitArgs(const ArgT& ... args) const {
        return sizeof...(ArgT) == m_argumentTypeIds.size() && fitArgsInternal<ArgT...>(0, args ...);
    }

    const std::vector<TypeId>& argumentTypeIds() const { return m_argumentTypeIds; }
    TypeId resultTypeId() const { return m_resultTypeId; }

private:
    std::function<Value(const std::vector<AnyArg>& anyArgs)> m_function;
    std::vector<TypeId> m_argumentTypeIds;
    TypeId m_resultTypeId;

    template<typename ArgsTupleT, typename FuncT,  std::size_t... I>
    static auto call(const Reflection& reflection, FuncT&& func, const std::vector<AnyArg>& args, std::index_sequence<I...>) {
        return func(args[I].as<typename std::tuple_element<I, ArgsTupleT>::type>(reflection)...);
    }

    template <typename...>
    bool fitArgsInternal(size_t) const {
        return true;
    }

    template <typename FrontArgT, typename ... ArgT>
    bool fitArgsInternal(size_t index, const FrontArgT& frontArg, const ArgT& ... args) const {
        if constexpr (std::is_same<std::decay_t<FrontArgT>, AnyArg>::value)
            return frontArg.typeId() == m_argumentTypeIds[index] && fitArgsInternal<ArgT...>(index + 1, args...);
        else
            return getTypeId<FrontArgT>() == m_argumentTypeIds[index] && fitArgsInternal<ArgT...>(index + 1, args...);
    }
};

class Type {
public:
    Type(TypeId typeId, std::vector<Function> constructors)
        : m_typeId(typeId)
        , m_constructors(constructors)
    {}

    const std::vector<Function>& constructors() const {
        return m_constructors;
    }

    template <typename ... ArgT>
    Value construct(ArgT&& ... anyArgs) const {
        for (const auto& constructor : m_constructors)
            if (constructor.fitArgs<ArgT...>(anyArgs ...))
                return constructor(std::forward<ArgT>(anyArgs)...);
        throw std::runtime_error("Can't construct");
    }

    TypeId typeId() const { return m_typeId; }
private:
    TypeId m_typeId;
    std::vector<Function> m_constructors;
};

void Reflection::addType(const std::shared_ptr<Type>& type) {
    m_types.emplace(type->typeId(), type);
}

template <typename TypeT, typename ... ArgT>
void Reflection::registerType(const ArgT&...args) {
    auto type = std::make_shared<Type>(getTypeId<TypeT>(), std::vector<Function>{ args.template generate<TypeT>(shared_from_this()) ... });
    m_types.emplace(type->typeId(), type);
}

template <typename ... ArgT>
class Constructor {
    friend class Reflection;
    template <typename ResultT>
    Function generate(const std::shared_ptr<Reflection>& reflection) const {
        auto lambda = [](ArgT...args) { return ResultT(args...); };
        typedef ResultT (*Func) (ArgT...);
        return Function(reflection, static_cast<Func>(lambda) );
    }
};

// Test functions

int somePrettyFunction(int a, int b, int& result) {
    std::cout << a << " " << b << std::endl;
    result = a + b;
    return a + b;
}

void test(std::string str) {
    std::cout << str << std::endl;
}

void test() {
    auto reflection = std::make_shared<Reflection>();
    auto lambda = [](const char* cstr) { return std::string(cstr); };
    typedef std::string (*Func) (const char*);
    reflection->registerType<std::string>(
                            Constructor<const char*>(),
                            Constructor<std::string>(),
                            Constructor<size_t, char>());

    auto wrappedFunc1 = Function(reflection, &somePrettyFunction);
    int result = 0;
    std::cout << wrappedFunc1(10, 20, result).as<int>() << std::endl;
    std::cout << result << std::endl;
    auto wrappedFunc2 = Function(reflection, &test);
    wrappedFunc2("Hello, World!");
    wrappedFunc2(reflection->getType(getTypeId<std::string>())->construct(size_t(10), 'a'));
}

TEST_CASE("General") {
    test();
}
