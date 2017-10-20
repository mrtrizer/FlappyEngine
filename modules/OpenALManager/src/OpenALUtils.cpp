#include "OpenALUtils.h"

#include <unordered_map>
#include <sstream>
#include <stdexcept>
#include <al.h>

namespace flappy {

void checkOpenALErrors(const char * file, const char * func, int line) {
    std::unordered_map<ALenum, std::string> errCodeMap {
        {AL_INVALID_ENUM, "Unknown error"},
        {AL_INVALID_VALUE, "Invalid value"},
        {AL_INVALID_OPERATION, "Invalid operation"},
        {AL_OUT_OF_MEMORY, "Out of memory"},
    };
    ALenum errCode = alGetError();
    if(errCode != AL_NO_ERROR) {
        std::stringstream ss;
        ss << " OpenAL error " << errCodeMap[errCode] << "\nin " << file << ' ' << line << ' ' << func <<  std::endl;
        throw std::runtime_error(ss.str());
    }
}

void checkALCErrors(ALCdevice* device, const char * file, const char * func, int line) {
    std::unordered_map<ALenum, std::string> errCodeMap {
        {ALC_INVALID_ENUM, "Unknown error"},
        {ALC_INVALID_VALUE, "Invalid value"},
        {ALC_INVALID_CONTEXT, "Invalid context"},
        {ALC_INVALID_DEVICE, "Invalid device"},
        {ALC_OUT_OF_MEMORY, "Out of memory"},
    };
    ALenum errCode = alcGetError(device);
    if(errCode != AL_NO_ERROR) {
        std::stringstream ss;
        ss << "OpenAL ALC error " << errCodeMap[errCode] << "\nin " << file << ' ' << line << ' ' << func <<  std::endl;
        throw std::runtime_error(ss.str());
    }
}


} // flappy
