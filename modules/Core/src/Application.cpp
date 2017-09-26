#include "Application.h"

#include <Entity.h>

namespace flappy {

Application* Application::s_instance = nullptr;

Application::Application()
    : m_rootEntity(std::make_shared<Entity>())
{
    if (s_instance != nullptr)
        throw std::runtime_error("Can't create multiple Application instancies");
    s_instance = this;
}

Application::~Application() {
    s_instance = nullptr;
}

int Application::runThread(std::shared_ptr<Thread> thread) {
    setThread(thread);
    return thread->run(rootEntity());
}

Application& Application::instance() {
    if (s_instance == nullptr)
        throw std::runtime_error("Application is not initialized.");
    return *s_instance;
}

} // flappy
