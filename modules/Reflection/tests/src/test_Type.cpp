#include <iostream>
#include <catch.h>

#include "Type.hpp"

using namespace flappy;

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

static void testFunc(std::string str) {
    std::cout << str << std::endl;
}

TEST_CASE("Type") {
    auto reflection = std::make_shared<Reflection>();

    auto type = reflection->registerType<TestClass>(
                            MethodRef("testMethod", &TestClass::testMethod),
                            MethodRef("testMethodConst", &TestClass::testMethod)
                );

    auto stringType = reflection->registerType<std::string>(
                            ConstructorRef<const char*>(),
                            ConstructorRef<std::string>(),
                            ConstructorRef<size_t, char>(),
                            MethodRef<std::string, unsigned long>("capacity", [](std::string& obj){ return obj.capacity(); } )
                );

    TestClass testClass(30);
    REQUIRE(type->method("testMethod")(testClass, 10, 20).as<int>() == 6000);

    auto wrappedFunc2 = Function(*reflection, &testFunc);
    wrappedFunc2("Hello, World!");
    auto str = reflection->getType(getTypeId<std::string>())->construct(size_t(10), 'a');
    auto& strRef = str;
    wrappedFunc2(str);
    std::cout << reflection->getType(getTypeId<std::string>())->method("capacity")(strRef).as<unsigned long>() << std::endl;
}
