#include <iostream>

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

class Test2 : public EnableSelfHandle<Test2>, public ITest {
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

const size_t N = 100000;

int main(int argc, char** argv)
{
    std::vector<std::shared_ptr<Test2>> tests;
    for (size_t i = 0; i < N; ++i)
        tests.emplace_back(std::make_shared<Test2>(i, "Test!"));
    for (size_t i = 0; i < 100; ++i)
        for (const auto& iter : tests)
            iter->update();
}
