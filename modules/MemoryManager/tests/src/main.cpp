#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <ObjectPool.hpp>
#include <Handle.hpp>
#include <UnknownHandle.hpp>
#include <ObjectPoolDebugger.hpp>
#include <Heap.hpp>

using namespace fakeit;
using namespace std;

class ITest {
public:
    virtual ~ITest() = default;
    virtual int value() = 0;
};

class Test : public ITest {
public:
    Test(int i) : m_i(i) {}
    int value() override { return m_i; }
private:
    int m_i = 0;
};

class Test2 : public EnableSelfHandle<Test2> {
public:
    Test2(int i, std::string text) : m_i(i), m_text(text) {}
    int value() { return m_i; }
    Handle<Test2> extractHandle() {
        return selfHandle();
    }
private:
    int m_i = 0;
    std::string m_text;
};

TEST_CASE( "Heap") {
    auto testObject = Heap::create<Test>(10);
    REQUIRE(testObject->value() == 10);
}

TEST_CASE("Self handle") {
    auto testObject = Heap::create<Test2>(100, "Self heap");
    REQUIRE(testObject->extractHandle()->value() == 100);
}

TEST_CASE("StrongHandle::StrongHandle(StrongHandle<DerivedT>&& strongHandle)") {
    auto testObject1 = Heap::create<Test>(10);
    REQUIRE(testObject1->value() == 10);

    StrongHandle<ITest> testObject2 = Heap::create<Test>(100);
    REQUIRE(testObject2->value() == 100);
}

TEST_CASE("StrongHandle::operator=(StrongHandle<DerivedT>&& strongHandle)") {
    auto testObject1 = Heap::create<Test>(10);
    REQUIRE(testObject1->value() == 10);

    StrongHandle<ITest> testObject2 = Heap::create<Test>(100);
    REQUIRE(testObject2->value() == 100);

    testObject2 = std::move(testObject1);
    REQUIRE(testObject2->value() == 10);
    REQUIRE_THROWS(testObject1->value());
}


TEST_CASE("Handle::Handle(StrongHandle<DerivedT>& strongHandle)") {
    auto strongHandle = Heap::create<Test>(10);

    Handle<ITest> test1 (strongHandle);
    REQUIRE(test1->value() == 10);
}

TEST_CASE("Handle::operator=(StrongHandle<DerivedT>& handle)") {
    auto strongHandle = Heap::create<Test>(10);

    Handle<ITest> test1 (strongHandle);
    test1 = strongHandle.handle();
    REQUIRE(test1->value() == 10);
}

TEST_CASE("Handle::Handle(const Handle<DerivedT>& handle)") {
    auto strongHandle = Heap::create<Test>(10);

    auto test2 = strongHandle.handle();
    Handle<ITest> test3 = test2;
    REQUIRE(test3->value() == 10);
}

TEST_CASE("Handle::operator=(const Handle<DerivedT>& handle)") {
    auto strongHandle = Heap::create<Test>(10);

    auto test2 = strongHandle.handle();
    Handle<ITest> test3;
    test3 = test2;
    REQUIRE(test3->value() == 10);
}

TEST_CASE("Handle::Handle(Handle<DerivedT>&& handle)") {
    auto strongHandle = Heap::create<Test>(10);

    auto test2 = strongHandle.handle();
    Handle<ITest> test3 = std::move(test2);
    REQUIRE(test3->value() == 10);
    REQUIRE_THROWS(test2->value());
}

TEST_CASE("Handle::operator=(Handle<DerivedT>&& handle)") {
    auto strongHandle = Heap::create<Test>(10);

    auto test2 = strongHandle.handle();
    Handle<ITest> test3;
    test3 = std::move(test2);
    REQUIRE(test3->value() == 10);
    REQUIRE_THROWS(test2->value());
}

TEST_CASE("Handle size") {
    REQUIRE(sizeof(Handle<Test>) == sizeof(intptr_t));
}

TEST_CASE( "General flow test") {
    ObjectPool objectPool(64, 10);
    ObjectPoolDebugger debugger(objectPool);
    {
        auto a = objectPool.create<Test>(10);
        REQUIRE(debugger.getChankIndex(a) == 0);
        REQUIRE(a->value() == 10);

        debugger.printState();

        UnknownHandle unknown = a;
        REQUIRE(unknown.get<Test>()->value() == 10);

        Handle<Test> b = nullptr;
        {
            auto strongHandle = objectPool.create<Test>(20);
            REQUIRE(debugger.getChankIndex(strongHandle) == 1);
            strongHandle = nullptr;
            REQUIRE(!strongHandle.isValid());

            auto strongHandle2 = objectPool.create<Test2>(20, "Fuck");
            REQUIRE(debugger.getChankIndex(strongHandle2) == 1);
            auto b2 = strongHandle2.handle();
            b = strongHandle;

            debugger.printState();

            strongHandle = objectPool.create<Test>(30);
            REQUIRE(debugger.getChankIndex(strongHandle) == 2);

            debugger.printState();

            auto otherStrongHandle = std::move(strongHandle);
            REQUIRE(debugger.getChankIndex(otherStrongHandle) == 2);
            REQUIRE(b->value() == 30);

            debugger.printState();
        }
        REQUIRE_THROWS(b->value());

        debugger.printState();

        auto c = objectPool.create<Test>(30);
        REQUIRE(debugger.getChankIndex(c) == 1);
        REQUIRE(c->value() == 30);

        debugger.printState();
    }
}
