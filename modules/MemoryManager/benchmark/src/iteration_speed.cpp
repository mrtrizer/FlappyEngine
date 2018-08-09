#include <iostream>

#include <hayai.hpp>

#include <ObjectPool.hpp>
#include <Handle.hpp>
#include <AnyHandle.hpp>
#include <ObjectPoolDebugger.hpp>
#include <Heap.hpp>

using namespace std;

class ITest {
public:
    virtual ~ITest() = default;
    virtual void update() = 0;
};

class Test2 : public EnableSelfHandle<Test2>, public std::enable_shared_from_this<Test2>, public ITest {
public:
    Test2(int i, std::string text) : m_i(i), m_text(text) {}
    int value() { return m_i; }
    Handle<Test2> extractHandle() {
        return selfHandle();
    }
    void update() {
        ++m_i;
        m_text.replace(1, 1, "Y");
        m_array[m_i % 10] = m_i;
    }
private:
    int m_i = 0;
    std::string m_text;
    int m_array[10] = {0};
};

const size_t N = 1000;

BENCHMARK(ObjectPoolTests, CreationTest_ObjectPool, 100, 1)
{
    ObjectPool pool(sizeof(Test2), N);
    std::vector<StrongHandle<Test2>> tests;
    for (size_t i = 0; i < N; ++i)
        tests.emplace_back(pool.create<Test2>(i, "Test!"));
}

class ObjectPoolDestroyFixture :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
        tests.clear();
    }

    ObjectPool pool {sizeof(Test2), N};
    std::vector<StrongHandle<Test2>> tests;
};

BENCHMARK_F(ObjectPoolDestroyFixture, Creation_ObjectPool, 100, 1)
{
    for (size_t i = 0; i < N; ++i)
        tests.emplace_back(pool.create<Test2>(i, "Test!"));
}

BENCHMARK(ObjectPoolTests, CreationTest_SharedPtr, 100, 1)
{
    std::vector<std::shared_ptr<Test2>> tests;
    for (size_t i = 0; i < N; ++i)
        tests.emplace_back(std::make_shared<Test2>(i, "Test!"));
}

class ObjectPoolPrepareFixture :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
        for (size_t i = 0; i < N; ++i) {
            tests.emplace_back(pool.create<Test2>(i, "Test!"));
            tests1.emplace_back(pool1.create<Test2>(i, "Test!"));
            tests2.emplace_back(pool2.create<Test2>(i, "Test!"));
            tests3.emplace_back(pool3.create<Test2>(i, "Test!"));
        }
    }

    ObjectPool pool {sizeof(Test2), N};
    std::vector<StrongHandle<ITest>> tests;
    ObjectPool pool1 {sizeof(Test2), N};
    std::vector<StrongHandle<ITest>> tests1;
    ObjectPool pool2 {sizeof(Test2), N};
    std::vector<StrongHandle<ITest>> tests2;
    ObjectPool pool3 {sizeof(Test2), N};
    std::vector<StrongHandle<ITest>> tests3;
};

BENCHMARK_F(ObjectPoolPrepareFixture, Iterations_ObjectPool, 10, 1000)
{
    for (const auto& iter : tests)
        iter->update();
}

class SharedPtrListPrepareFixture :   public ::hayai::Fixture
{
public:
    virtual void SetUp()
    {
        for (size_t i = 0; i < N; ++i) {
            tests.emplace_back(std::make_shared<Test2>(i, "Test!"));
            tests1.emplace_back(std::make_shared<Test2>(i, "Test!"));
            tests2.emplace_back(std::make_shared<Test2>(i, "Test!"));
            tests3.emplace_back(std::make_shared<Test2>(i, "Test!"));
        }
    }

    std::vector<std::shared_ptr<ITest>> tests;
    std::vector<std::shared_ptr<ITest>> tests1;
    std::vector<std::shared_ptr<ITest>> tests2;
    std::vector<std::shared_ptr<ITest>> tests3;
};

BENCHMARK_F(SharedPtrListPrepareFixture, Iterations_SharedPtr, 10, 1000)
{
    for (const auto& iter : tests)
        iter->update();
}
