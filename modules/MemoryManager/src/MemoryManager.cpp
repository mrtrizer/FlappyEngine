#include "MemoryManager.hpp"

namespace flappy {

MemoryManager::MemoryManager()
    : m_generator(m_randomDevice())
    , m_distribution(0, std::numeric_limits<ObjectId>::max())
{}

} // flappy
