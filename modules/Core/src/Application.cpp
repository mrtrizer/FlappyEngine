#include "Application.h"

namespace flappy {

Application* Application::s_instance = nullptr;

Application::Application() {
    if (s_instance != nullptr)
        throw std::runtime_error("Can't create multiple Application instancies");
    s_instance = this;
}

Application::~Application() {
    s_instance = nullptr;
}

void Application::runThread(std::shared_ptr<Thread> thread) {
    setThread(thread);
    thread->run();
}

Application& Application::instance() {
    if (s_instance == nullptr)
        throw std::runtime_error("Application is not initialized.");
    return *s_instance;
}

} // flappy
