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
    
    std::string testMethodConstRefArg(const std::string& str) {
        return str;
    }
    
    std::string testMethodConstPtrArg(const std::string* str) {
        return *str;
    }
    
    int* newArray(int n) {
        auto array = new int[n];
        for (int i = 0; i < n; ++i)
            array[i] = i;
        return array;
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

    auto type = reflection->registerType<TestClass>("TestClass")
            .addConstructor<TestClass, int>()
            .addFunction("testMethod", &TestClass::testMethod)
            .addFunction("testMethodConst", &TestClass::testMethod)
            .addFunction("c", &TestClass::c);

    TestClass testClass(30);
    REQUIRE(type.function("testMethod")(testClass, 10, 20).as<int>() == 6000);

    auto wrappedFunc2 = Function(*reflection, &testFunc);
    wrappedFunc2("Hello, World!");
    auto str = reflection->getType(getTypeId<std::string>()).constructOnStack(size_t(10), 'a');
    auto& strRef = str;
    wrappedFunc2(str);
    std::cout << reflection->getType(getTypeId<std::string>()).function("capacity")(strRef).as<unsigned long>() << std::endl;
}

TEST_CASE("Type constructors") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());

    auto type = reflection->registerType<TestClass>("TestClass")
            .addConstructor<TestClass, int>()
            .addFunction("testMethod", &TestClass::testMethod)
            .addFunction("testMethodConst", &TestClass::testMethodConst)
            .addFunction("testMethodConstRefArg", &TestClass::testMethodConstRefArg)
            .addFunction("testMethodConstPtrArg", &TestClass::testMethodConstPtrArg)
            .addFunction("newArray", &TestClass::newArray)
            .addFunction("c", &TestClass::c)
            .addField("m_c", &TestClass::m_c);
    REQUIRE(type.functionMap().find("testMethod") != type.functionMap().end());

    auto typeSharedPtr = reflection->registerType<std::shared_ptr<TestClass>>("std::shared_ptr<TestClass>")
            .addConstructor<std::shared_ptr<TestClass>, TestClass*>()
            .addFunction<std::shared_ptr<TestClass>, TestClass*>("get", [](auto v) { return v.get(); });

    auto rawPointer1 = type.constructOnHeap(10);
    auto value = typeSharedPtr.constructOnStack(rawPointer1);
    auto rawPointer = typeSharedPtr.function("get")(value);
    auto ref = rawPointer.deref();
    REQUIRE(type.function("c")(ref).as<int>() == 10);
    REQUIRE(type.field("m_c").getValue(ref).as<int>() == 10);
    type.field("m_c").setValue(ref, 20);
    REQUIRE(type.field("m_c").getValue(ref).as<int>() == 20);
    REQUIRE(type.function("testMethod")(ref, 10, 20).as<int>() == 4000);
    REQUIRE(type.function("testMethodConst")(ref, 10, 20).as<int>() == 4000);
    REQUIRE(type.function("testMethodConstRefArg")(ref, "test").as<std::string>() == "test");
    Value testStr(std::string("test"));
    REQUIRE(type.function("testMethodConstPtrArg")(ref, testStr.addressOf()).as<std::string>() == "test");
    auto arrayPtr = type.function("newArray")(ref, 2);
    REQUIRE(arrayPtr.as<int*>()[1] == 1);
    delete arrayPtr.as<int*>();
}
