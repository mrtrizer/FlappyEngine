#include "flappyapp.h"

namespace flappy {

using namespace std;

FlappyApp::FlappyApp(int argc, const char **argv):
    m_args(argc)
{
    if (argv == nullptr) {
        m_args[0] = "flappy";
    } else {
        for (int i = 0; i < argc; i++)
            m_args[i] = argv[i];
    }
}

} // flappy
