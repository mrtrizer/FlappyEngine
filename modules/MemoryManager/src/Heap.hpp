#pragma once

#include "ObjectPool.hpp"

namespace flappy {

template <typename ObjectT>
class ConcreteHeap {
public:
    static ConcreteHeap& instance() {
        static ConcreteHeap concreteHeap;
        return concreteHeap;
    }

    ObjectPool& objectPool() { return m_objectPool; }

private:
    ConcreteHeap()
        : m_objectPool(sizeof(ObjectT), 10) // FIXME: Maybe prefer lower initial capacity to not waste memory
    {}

    ObjectPool m_objectPool;
};

class Heap {
public:
    Heap() = delete;
    Heap(const Heap&) = delete;
    Heap& operator=(const Heap&) = delete;
    Heap(Heap&&) = delete;
    Heap& operator=(Heap&&) = delete;
    ~Heap() = delete;

    template <typename DataT, typename...Args>
    [[nodiscard]] static StrongHandle<DataT> create(Args ... args) {
        return ConcreteHeap<DataT>::instance().objectPool().template create<DataT>(std::forward<Args>(args)...);
    }
};

} // flappy
