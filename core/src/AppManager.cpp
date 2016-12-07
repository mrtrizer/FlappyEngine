#include "AppManager.h"

namespace flappy {

using namespace std;

AppManager::AppManager(int argc, const char * const *argv):
    m_args(argc)
{
    if (argv == nullptr) {
        m_args[0] = std::string("flappy");
    } else {
        for (int i = 0; i < argc; i++)
            m_args[i] = std::string(argv[i]);
    }
}

} // flappy
