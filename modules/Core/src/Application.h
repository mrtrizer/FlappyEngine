#pragma once

#include <SafePtr.h>

#include "Thread.h"

namespace flappy {

class Entity;

/// The main purpose of an Application class in
/// thread managment according to platfrom implementation
class Application {
public:
    Application();
    virtual ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) & = delete;

    void runThread(std::shared_ptr<Thread> thread);
    virtual SafePtr<Thread> getThread() = 0;

    static Application& instance();

    SafePtr<Entity> rootEntity() { return m_rootEntity; }

private:
    virtual void setThread(std::shared_ptr<Thread> thread) = 0;

    static Application* s_instance;
    std::shared_ptr<Entity> m_rootEntity;
};

} //flappy
