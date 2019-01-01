#pragma once

#include "MemoryManager.hpp"

namespace flappy {

class Heap {
public:
    Heap(const Heap&) = delete;
    Heap& operator=(const Heap&) = delete;
    Heap(Heap&&) = delete;
    Heap& operator=(Heap&&) = delete;

        template <typename DataT, typename...ArgT>
    [[nodiscard]] static StrongHandle<DataT> create(ArgT&& ... args) {
        return memoryManager().template create<DataT>(std::forward<ArgT>(args)...);
    }
    
    static MemoryManager& memoryManager() { return instance().m_memoryManager; }
    
private:
    Heap() = default;
    ~Heap() = default;

    MemoryManager m_memoryManager;
    
    static Heap& instance() {
        static Heap heap;
        return heap;
    }
};

} // flappy
