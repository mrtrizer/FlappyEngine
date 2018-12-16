#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.hpp>

#include <OpenAL.h>
#include <al.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace flappy;
using namespace fakeit;
using namespace std;

#define BUFFER_SIZE   32768     // 32 KB buffers

void LoadOGG(char *fileName, vector < char > &buffer, ALenum &format, ALsizei &freq)
{
  int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
  int bitStream;
  long bytes;
  char array[BUFFER_SIZE];    // Local fixed size array
  FILE *f;

  // Open for binary reading
  f = fopen(fileName, "rb");

  vorbis_info *pInfo;
    OggVorbis_File oggFile;

    ov_open(f, &oggFile, NULL, 0);

    // Get some information about the OGG file
      pInfo = ov_info(&oggFile, -1);

      // Check the number of channels... always use 16-bit samples
      if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
      else
        format = AL_FORMAT_STEREO16;
      // end if

      // The frequency of the sampling rate
      freq = pInfo->rate;

      do {
          // Read up to a buffer's worth of decoded sound data
          bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
          // Append to end of buffer
          buffer.insert(buffer.end(), array, array + bytes);
        } while (bytes > 0);

      ov_clear(&oggFile);
}

void test() {
    ALCdevice *device;
    device = alcOpenDevice(NULL);
    if (!device)
        throw std::runtime_error("Can't create a device ");

    auto context = alcCreateContext(device, NULL);

    if (!alcMakeContextCurrent(context))
        throw std::runtime_error("Can't make a context current");

    ALint state;                // The state of the sound source
    ALuint bufferID;            // The OpenAL sound buffer ID
    ALuint sourceID;            // The OpenAL sound source
    ALenum format;              // The sound data format
    ALsizei freq;               // The frequency of the sound data
    vector < char > bufferData; // The sound buffer data from file

    LoadOGG("resources/boom.ogg", bufferData, format, freq);

    // Create sound buffer and source
    alGenBuffers(1, &bufferID);
    alGenSources(1, &sourceID);

    // Set the source and listener to the same location
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);

    // Upload sound data to buffer
    alBufferData(bufferID, format, &bufferData[0], static_cast < ALsizei > (bufferData.size()), freq);

    // Attach sound buffer to source
    alSourcei(sourceID, AL_BUFFER, bufferID);

    // Finally, play the sound!!!
    alSourcePlay(sourceID);
    // This is a busy wait loop but should be good enough for example purpose
    do {
        // Query the state of the souce
        alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    } while (state != AL_STOPPED);

    // Clean up sound buffer and source
    alDeleteBuffers(1, &bufferID);
    alDeleteSources(1, &sourceID);

}

TEST_CASE("OpenAL") {
    test();
}
