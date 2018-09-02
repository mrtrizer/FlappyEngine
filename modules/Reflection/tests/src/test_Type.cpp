#include <iostream>
#include <catch.h>

#include "Type.hpp"
#include "BasicTypesReflection.hpp"

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
    int c() const {
        return m_c;
    }

    int m_c = 0;
};

static void testFunc(std::string str) {
    std::cout << str << std::endl;
}

TEST_CASE("Type") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());

    auto type = reflection->registerType<TestClass>("TestClass",
                            ConstructorRef<int>(),
                            MethodRef("testMethod", &TestClass::testMethod),
                            MethodRef("testMethodConst", &TestClass::testMethod)
                );

    TestClass testClass(30);
    REQUIRE(type.method("testMethod")(testClass, 10, 20).as<int>() == 6000);

    auto wrappedFunc2 = Function(*reflection, &testFunc);
    wrappedFunc2("Hello, World!");
    auto str = reflection->getType(getTypeId<std::string>()).constructOnStack(size_t(10), 'a');
    auto& strRef = str;
    wrappedFunc2(str);
    std::cout << reflection->getType(getTypeId<std::string>()).method("capacity")(strRef).as<unsigned long>() << std::endl;
}

TEST_CASE("Type constructors") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());

    auto type = reflection->registerType<TestClass>("TestClass",
                            ConstructorRef<int>(),
                            MethodRef("testMethod", &TestClass::testMethod),
                            MethodRef("testMethodConst", &TestClass::testMethod),
                            MethodRef("c", &TestClass::c)
                );

    auto typePtr = reflection->registerType<std::shared_ptr<TestClass>>("std::shared_ptr<TestClass>",
                            MethodRef<std::shared_ptr<TestClass>, TestClass*>("get", [](std::shared_ptr<TestClass>& v) { return v.get(); })
                );

    auto value = type.constructSharedPtr(10);
    auto rawPointer = typePtr.method("get")(value);
    auto ref = rawPointer.deref();
    REQUIRE(type.method("c")(ref).as<int>() == 10);
}
