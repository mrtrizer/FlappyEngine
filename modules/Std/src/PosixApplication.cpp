#include "PosixApplication.h"

namespace flappy {

SafePtr<Thread> PosixApplication::getThread() {
    return m_thread;
}

void PosixApplication::setThread(std::shared_ptr<Thread> thread) {
    m_thread = thread;
}

} // flappy
