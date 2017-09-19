#pragma once

#include <Application.h>

namespace flappy {

class PosixApplication: public Application {
public:
    virtual SafePtr<Thread> getThread() final;

private:
    virtual void setThread(std::shared_ptr<Thread> thread) final;

    std::shared_ptr<Thread> m_thread;
};

} // flappy
