#pragma once

#include "Handle.hpp"

namespace flappy {

template <typename ObjectT>
class EnableSelfHandle {
    friend class ObjectPool; // to set m_selfChankPtr
protected:
    EnableSelfHandle() {
        // Take a note that m_selfChankPtr is initialized outside!
        // EnableSelfHandle is constructed, before derived class and provides selfHandle().
        // So, derived class can access selfHandle() in constructor.
        // There are other ways to implement this, like static variable, but it's not thread safe. Other
        // implementation not optimal. If you have a better idea, email me please mrtrizer@gmail.com.
        m_handle = Handle<ObjectT>(m_selfChankPtr);
    }
    Handle<ObjectT> selfHandle() { return m_handle; }
private:
    // WARNING: Do NOT intialize m_selfChankPtr! It's inititlized outside, before construction. Yup, its crazy.
    Chank* m_selfChankPtr;
    Handle<ObjectT> m_handle;
};

} // flappy
