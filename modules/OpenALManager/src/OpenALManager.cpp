#include "OpenALManager.h"

#include <AL/alut.h>

namespace flappy {

OpenALManager::OpenALManager()
{
    ALuint helloBuffer, helloSource;
    int argc = 0;
    char* argv[0];
    alutInit (&argc, argv);
    helloBuffer = alutCreateBufferHelloWorld ();
    alGenSources (1, &helloSource);
    alSourcei (helloSource, AL_BUFFER, helloBuffer);
    alSourcePlay (helloSource);
    alutSleep (1);
    alutExit ();
}

} // flappy
