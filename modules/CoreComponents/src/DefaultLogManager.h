#pragma once

#include "ILogManager.h"

namespace flappy {

class DefaultLogManager : public ILogManager {
public:
    DefaultLogManager();

    void log(const char* format, ...) final;

private:
    std::string m_outputBuff;
};

} // flappy
