#pragma once

#include <string>

#include <alc.h>

namespace flappy {

void checkOpenALErrors(const char * file, const char * func, int line);

void checkALCErrors(ALCdevice* device, const char * file, const char * func, int line);

} // flappy

#define CHECK_AL_ERROR checkOpenALErrors(__FILE__, __FUNCTION__, __LINE__)
#define CHECK_ALC_ERROR(device) checkALCErrors((device), __FILE__, __FUNCTION__, __LINE__)
