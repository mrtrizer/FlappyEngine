#include <iostream>
#include <catch.h>

#include "Reflection.hpp"
#include "ConstructorRef.hpp"
#include "MethodRef.hpp"
#include "Type.hpp"

using namespace flappy;

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
